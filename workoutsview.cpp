#include "workoutsview.h"
#include "ui_workoutsview.h"
#include "./AI/aiworkoutgenerator.h"
#include "./AI/aiworkoutplan.h"
#include <QMessageBox>
#include <QAction>

// ======================================================
// KONSTRUKTOR
// ======================================================
WorkoutsView::WorkoutsView(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::WorkoutsView)
{
    ui->setupUi(this);

    trainingManager = new TrainingManager(DatabaseManager::instance());

    // ============================
    // Typ ćwiczenia
    // ============================
    ui->exerciseTypeComboBox->clear();
    ui->exerciseTypeComboBox->addItem("Siłowe", "strength");
    ui->exerciseTypeComboBox->addItem("Cardio", "cardio");

    // ============================
    // Domyślnie: Strength
    // ============================
    loadExercises("strength");

    // ============================
    // Zmiana typu ćwiczenia
    // ============================
    connect(ui->exerciseTypeComboBox, &QComboBox::currentIndexChanged,
            this, [this](int index)
            {
                QString type =
                    ui->exerciseTypeComboBox->itemData(index).toString();
                loadExercises(type);
            });

    // ============================
    // Przyciski
    // ============================
    connect(ui->addExerciseButton, &QPushButton::clicked,
            this, &WorkoutsView::onAddExerciseClicked);

    connect(ui->saveWorkoutButton, &QPushButton::clicked,
            this, &WorkoutsView::onSaveWorkoutClicked);

    connect(ui->generateAiWorkoutButton, &QPushButton::clicked,
            this, &WorkoutsView::onGenerateAiWorkoutClicked);
}

void WorkoutsView::onGenerateAiWorkoutClicked()
{
    currentPlan.clear();
    ui->addedExercisesList->clear();

    // 🔥 ZBIERZ ĆWICZENIA Z COMBOBOXA
    QStringList allowedExercises;
    for (int i = 0; i < ui->exerciseComboBox->count(); ++i)
    {
        allowedExercises << ui->exerciseComboBox->itemText(i);
    }

    auto* generator = new AIWorkoutGenerator(this);

    connect(generator, &AIWorkoutGenerator::planReady,
            this, &WorkoutsView::onAiPlanReady);

    connect(generator, &AIWorkoutGenerator::error,
            this, [](const QString& err)
            {
                QMessageBox::critical(nullptr, "AI error", err);
            });

    // 🔥 PRZEKAŻ LISTĘ DO AI
    generator->generateFBW(allowedExercises);
}


void WorkoutsView::onAiPlanReady(const AIWorkoutPlan& plan)
{
    ui->workoutNameLineEdit->setText(plan.name);

    for (const auto& ex : plan.exercises)
    {
        int defId =
            trainingManager->getExerciseDefinitionId(ex.name);

        if (defId == -1)
            continue;

        WorkoutExercisePlan p;
        p.exerciseDefId = defId;
        p.type = ex.type;

        if (ex.type == "strength")
        {
            p.sets = ex.sets;
            p.reps = ex.reps;
            p.weight = ex.weight;

            ui->addedExercisesList->addItem(
                QString("%1 | %2x%3 | %4 kg")
                    .arg(ex.name)
                    .arg(ex.sets)
                    .arg(ex.reps)
                    .arg(ex.weight)
                );
        }
        else
        {
            p.duration = ex.duration;
            p.distance = ex.distance;

            ui->addedExercisesList->addItem(
                QString("%1 | %2 min | %3 km")
                    .arg(ex.name)
                    .arg(ex.duration)
                    .arg(ex.distance)
                );
        }

        currentPlan.append(p);
    }

    QMessageBox::information(this, "AI", "Trening wygenerowany przez AI");
}


WorkoutsView::~WorkoutsView()
{
    delete ui;
}
void WorkoutsView::loadExercises(const QString& type)
{
    ui->exerciseComboBox->clear();

    QStringList exercises =
        trainingManager->getExerciseNamesByType(type);

    ui->exerciseComboBox->addItems(exercises);
}
// ======================================================
// USER
// ======================================================
void WorkoutsView::setUserData(UserModel *currentUser)
{
    this->currentUser = currentUser;
}

// ======================================================
// DODAWANIE / EDYCJA ĆWICZENIA
// ======================================================
void WorkoutsView::onAddExerciseClicked()
{
    QString exerciseName = ui->exerciseComboBox->currentText();
    QString type =
        ui->exerciseTypeComboBox->currentData().toString();


    if (exerciseName.isEmpty())
        return;

    int exerciseDefId =
        trainingManager->getExerciseDefinitionId(exerciseName);

    if (exerciseDefId == -1)
    {
        QMessageBox::warning(this, "Błąd", "Ćwiczenie nie istnieje w bazie");
        return;
    }

    // ==========================
    // STRENGTH
    // ==========================
    if (type == "strength")
    {
        WorkoutExercisePlan plan;
        plan.exerciseDefId = exerciseDefId;
        plan.type = "strength";
        plan.sets = ui->setsSpinBox->value();
        plan.reps = ui->repsSpinBox->value();
        plan.weight = ui->weightSpinBox->value();

        currentPlan.append(plan);

        ui->addedExercisesList->addItem(
            QString("%1 | %2x%3 | %4 kg")
                .arg(exerciseName)
                .arg(plan.sets)
                .arg(plan.reps)
                .arg(plan.weight)
            );
    }
    // ==========================
    // CARDIO
    // ==========================
    else
    {
        WorkoutExercisePlan plan;
        plan.exerciseDefId = exerciseDefId;
        plan.type = "cardio";
        plan.duration = ui->durationSpinBox->value();
        plan.distance = ui->distanceSpinBox->value();

        currentPlan.append(plan);

        ui->addedExercisesList->addItem(
            QString("%1 | %2 min | %3 km")
                .arg(exerciseName)
                .arg(plan.duration)
                .arg(plan.distance)
            );
    }
}


// ======================================================
// USUWANIE ĆWICZENIA
// ======================================================
void WorkoutsView::onRemoveExerciseClicked()
{
    int row = ui->addedExercisesList->currentRow();
    if (row < 0)
        return;

    currentPlan.removeAt(row);
    delete ui->addedExercisesList->takeItem(row);
}

// ======================================================
// ZAPIS TRENINGU
// ======================================================
void WorkoutsView::onSaveWorkoutClicked()
{
    QString workoutName = ui->workoutNameLineEdit->text().trimmed();

    if (workoutName.isEmpty())
    {
        QMessageBox::warning(this, "Błąd", "Podaj nazwę treningu");
        return;
    }

    if (currentPlan.isEmpty())
    {
        QMessageBox::warning(this, "Błąd", "Dodaj ćwiczenia do treningu");
        return;
    }

    if (!trainingManager->saveWorkoutWithPlan(workoutName, currentPlan))
    {
        QMessageBox::critical(this, "Błąd", "Nie udało się zapisać treningu");
        return;
    }

    QMessageBox::information(this, "OK", "Trening zapisany do bazy");

    // ===== RESET =====
    currentPlan.clear();
    ui->addedExercisesList->clear();
    ui->workoutNameLineEdit->clear();
}

