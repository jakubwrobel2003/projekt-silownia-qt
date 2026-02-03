#include "workoutsview.h"
#include "ui_workoutsview.h"
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QDesktopServices> // Do otwierania linków
#include <QUrl>

WorkoutsView::WorkoutsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkoutsView)
{
    ui->setupUi(this);

    trainingManager = new TrainingManager(DatabaseManager::instance());

    // --- KONFIGURACJA TABELI ---
    ui->exercisesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->exercisesTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    // --- COMBOBOX TYP ---
    ui->exerciseTypeComboBox->addItem("Siłowe", "strength");
    ui->exerciseTypeComboBox->addItem("Cardio", "cardio");

    // --- INICJALIZACJA LINKÓW WIDEO ---
    initVideoLinks();

    // Domyślne ładowanie
    loadExercises("strength");
    toggleInputFields();

    // --- SYGNAŁY ---
    connect(ui->exerciseTypeComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        QString type = ui->exerciseTypeComboBox->itemData(index).toString();
        loadExercises(type);
        toggleInputFields();
        ui->lblVideoLink->clear(); // Czyść link przy zmianie typu
    });

    // Czyść link przy zmianie ćwiczenia
    connect(ui->exerciseComboBox, &QComboBox::currentIndexChanged, this, [this]() {
        ui->lblVideoLink->clear();
    });

    connect(ui->addExerciseButton, &QPushButton::clicked, this, &WorkoutsView::onAddExerciseClicked);
    connect(ui->saveWorkoutButton, &QPushButton::clicked, this, &WorkoutsView::onSaveWorkoutClicked);
    connect(ui->generateAiWorkoutButton, &QPushButton::clicked, this, &WorkoutsView::onGenerateAiWorkoutClicked);

    // 🔥 OBSŁUGA PRZYCISKU WIDEO
    connect(ui->btnShowVideo, &QPushButton::clicked, this, &WorkoutsView::onShowVideoClicked);
}

WorkoutsView::~WorkoutsView()
{
    delete ui;
}

void WorkoutsView::setUserData(UserModel *currentUser)
{
    this->currentUser = currentUser;
}

void WorkoutsView::loadExercises(const QString &type)
{
    ui->exerciseComboBox->clear();
    QStringList exercises = trainingManager->getExerciseNamesByType(type);
    ui->exerciseComboBox->addItems(exercises);
}

// ======================================================
// LINKI DO WIDEO (Twoje 6 ćwiczeń)
// ======================================================
void WorkoutsView::initVideoLinks()
{
    exerciseLinks["Bench Press"] = "https://www.youtube.com/watch?v=rT7DgCr-3pg";
    exerciseLinks["Squat"] = "https://www.youtube.com/watch?v=gcNh17Ckjgg";
    exerciseLinks["Deadlift"] = "https://www.youtube.com/watch?v=op9kVnSso6Q";
    exerciseLinks["Overhead Press"] = "https://www.youtube.com/watch?v=QAQ64hK4Xxs";
    exerciseLinks["Pull Ups"] = "https://www.youtube.com/watch?v=eGo4IYlbE5g";
    exerciseLinks["Treadmill Run"] = "https://www.youtube.com/watch?v=8i3VwdNKbnI";
}

void WorkoutsView::onShowVideoClicked()
{
    QString currentExercise = ui->exerciseComboBox->currentText();

    if (exerciseLinks.contains(currentExercise)) {
        QString url = exerciseLinks[currentExercise];
        // Wyświetlamy klikalny link
        QString html = QString("<a href='%1' style='color: #4FC3F7; font-weight: bold;'>👉 Zobacz instrukcję na YouTube</a>").arg(url);
        ui->lblVideoLink->setText(html);
    } else {
        ui->lblVideoLink->setText("Brak wideo dla tego ćwiczenia.");
    }
}

// ======================================================
// UKRYWANIE PÓL
// ======================================================
void WorkoutsView::toggleInputFields()
{
    QString type = ui->exerciseTypeComboBox->currentData().toString();

    if (type == "strength") {
        ui->strengthParamsWidget->setVisible(true);
        ui->cardioParamsWidget->setVisible(false);
    } else {
        ui->strengthParamsWidget->setVisible(false);
        ui->cardioParamsWidget->setVisible(true);
    }
}

// ======================================================
// ODŚWIEŻANIE TABELI
// ======================================================
void WorkoutsView::updateExerciseTable()
{
    ui->exercisesTable->setRowCount(0);

    for (int i = 0; i < currentPlan.size(); ++i) {
        const auto &plan = currentPlan[i];

        // Pobieramy nazwę lokalną funkcją pomocniczą
        QString name = getExerciseNameById(plan.exerciseDefId);

        int row = ui->exercisesTable->rowCount();
        ui->exercisesTable->insertRow(row);

        ui->exercisesTable->setItem(row, 0, new QTableWidgetItem(name));
        ui->exercisesTable->setItem(row, 1, new QTableWidgetItem(plan.type == "strength" ? "Siłowe" : "Cardio"));

        if (plan.type == "strength") {
            ui->exercisesTable->setItem(row, 2, new QTableWidgetItem(QString::number(plan.sets)));
            ui->exercisesTable->setItem(row, 3, new QTableWidgetItem(QString::number(plan.reps)));
            ui->exercisesTable->setItem(row, 4, new QTableWidgetItem(QString::number(plan.weight) + " kg"));
        } else {
            ui->exercisesTable->setItem(row, 2, new QTableWidgetItem(QString::number(plan.duration) + " min"));
            ui->exercisesTable->setItem(row, 3, new QTableWidgetItem(QString::number(plan.distance) + " km"));
            ui->exercisesTable->setItem(row, 4, new QTableWidgetItem("-"));
        }

        // Przycisk USUŃ
        QPushButton *btnRemove = new QPushButton("Usuń");
        btnRemove->setStyleSheet("background-color: #FF5252; color: white; border: none; border-radius: 4px; font-weight: bold; padding: 5px;");
        btnRemove->setCursor(Qt::PointingHandCursor);

        connect(btnRemove, &QPushButton::clicked, this, [this, i]() {
            removeExerciseAt(i);
        });

        ui->exercisesTable->setCellWidget(row, 5, btnRemove);
    }
}

void WorkoutsView::removeExerciseAt(int index)
{
    if (index >= 0 && index < currentPlan.size()) {
        currentPlan.removeAt(index);
        updateExerciseTable();
    }
}

// ======================================================
// POMOCNICZA FUNKCJA (Zastępuje brak w Managerze)
// ======================================================
QString WorkoutsView::getExerciseNameById(int id)
{
    // Bezpośrednie zapytanie, skoro nie chcesz ruszać managera
    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT name FROM exercise_definitions WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        return q.value(0).toString();
    }
    return "Nieznane";
}

// ======================================================
// DODAWANIE
// ======================================================
void WorkoutsView::onAddExerciseClicked()
{
    QString exerciseName = ui->exerciseComboBox->currentText();
    QString type = ui->exerciseTypeComboBox->currentData().toString();

    if (exerciseName.isEmpty()) return;

    int defId = trainingManager->getExerciseDefinitionId(exerciseName);
    if (defId == -1) {
        QMessageBox::warning(this, "Błąd", "Nie znaleziono ćwiczenia!");
        return;
    }

    WorkoutExercisePlan plan;
    plan.exerciseDefId = defId;
    plan.type = type;

    if (type == "strength") {
        plan.sets = ui->setsSpinBox->value();
        plan.reps = ui->repsSpinBox->value();
        plan.weight = ui->weightSpinBox->value();
    } else {
        plan.duration = ui->durationSpinBox->value();
        plan.distance = ui->distanceSpinBox->value();
    }

    currentPlan.append(plan);
    updateExerciseTable();
}

// ======================================================
// ZAPIS
// ======================================================
void WorkoutsView::onSaveWorkoutClicked()
{
    QString name = ui->workoutNameLineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Podaj nazwę treningu!");
        return;
    }
    if (currentPlan.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Dodaj chociaż jedno ćwiczenie!");
        return;
    }

    if (trainingManager->saveWorkoutWithPlan(name, currentPlan)) {
        QMessageBox::information(this, "Sukces", "Trening zapisany!");
        currentPlan.clear();
        updateExerciseTable();
        ui->workoutNameLineEdit->clear();
    } else {
        QMessageBox::critical(this, "Błąd", "Błąd zapisu do bazy.");
    }
}

// ======================================================
// AI
// ======================================================
void WorkoutsView::onGenerateAiWorkoutClicked()
{
    currentPlan.clear();
    QStringList allowedExercises;
    for (int i = 0; i < ui->exerciseComboBox->count(); ++i) {
        allowedExercises << ui->exerciseComboBox->itemText(i);
    }

    auto *generator = new AIWorkoutGenerator(this);
    connect(generator, &AIWorkoutGenerator::planReady, this, &WorkoutsView::onAiPlanReady);
    connect(generator, &AIWorkoutGenerator::error, this, [](const QString &err) {
        QMessageBox::critical(nullptr, "AI Error", err);
    });

    generator->generateFBW(allowedExercises);
}

void WorkoutsView::onAiPlanReady(const AIWorkoutPlan &plan)
{
    ui->workoutNameLineEdit->setText(plan.name);

    for (const auto &ex : plan.exercises) {
        int defId = trainingManager->getExerciseDefinitionId(ex.name);
        if (defId == -1) continue;

        WorkoutExercisePlan p;
        p.exerciseDefId = defId;
        p.type = ex.type;

        if (ex.type == "strength") {
            p.sets = ex.sets;
            p.reps = ex.reps;
            p.weight = ex.weight;
        } else {
            p.duration = ex.duration;
            p.distance = ex.distance;
        }
        currentPlan.append(p);
    }
    updateExerciseTable();
    QMessageBox::information(this, "AI", "Trening wygenerowany pomyślnie!");
}
