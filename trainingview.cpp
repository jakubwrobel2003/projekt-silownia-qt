#include "trainingview.h"
#include "ui_trainingview.h"
#include "./Database/databasemanager.h"
#include <QMessageBox>
#include <QDate>

trainingview::trainingview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::trainingview)
{
    ui->setupUi(this);
}

trainingview::~trainingview() {
    delete ui;
}

void trainingview::setUserData(UserModel* user) {
    currentUser = user;
    refreshWorkoutList();
}

void trainingview::refreshWorkoutList() {
    ui->listWorkouts->clear();
    QSqlQuery q(DatabaseManager::instance().database());
    q.exec("SELECT id, name FROM workouts");

    while (q.next()) {
        QListWidgetItem* item = new QListWidgetItem(q.value(1).toString());
        item->setData(Qt::UserRole, q.value(0).toInt()); // Chowamy ID w elemencie listy
        ui->listWorkouts->addItem(item);
    }
}

void trainingview::clearForm() {
    activeInputs.clear();
    QLayoutItem *child;
    while ((child = ui->dynamicExerciseLayout->takeAt(0)) != nullptr) {
        if(child->layout()) {
            QLayoutItem *subChild;
            while((subChild = child->layout()->takeAt(0)) != nullptr) {
                delete subChild->widget();
                delete subChild;
            }
        }
        delete child->widget();
        delete child;
    }
}

void trainingview::on_listWorkouts_itemClicked() {
    clearForm();
    QListWidgetItem* item = ui->listWorkouts->currentItem();
    if (!item) return;

    selectedWorkoutId = item->data(Qt::UserRole).toInt();

    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT ed.id, ed.name FROM workout_exercise_plan p "
              "JOIN exercise_definitions ed ON p.exercise_def_id = ed.id "
              "WHERE p.workout_id = :wid");
    q.bindValue(":wid", selectedWorkoutId);

    if (q.exec()) {
        while (q.next()) {
            int exId = q.value(0).toInt();
            QString name = q.value(1).toString();

            // Tworzymy wiersz formularza
            QHBoxLayout* row = new QHBoxLayout();
            QLabel* lblName = new QLabel(name);
            QLineEdit* editWeight = new QLineEdit();
            editWeight->setPlaceholderText("Ciężar (kg)");
            QLineEdit* editReps = new QLineEdit();
            editReps->setPlaceholderText("Powtórzenia");

            row->addWidget(lblName, 2);
            row->addWidget(editWeight, 1);
            row->addWidget(editReps, 1);

            ui->dynamicExerciseLayout->addLayout(row);

            // Zapamiętujemy wskaźniki, żeby móc odczytać dane przy zapisie
            activeInputs.append({exId, editWeight, editReps});
        }
    }
}

void trainingview::on_btnSaveWorkout_clicked() {
    if (!currentUser || selectedWorkoutId == -1 || activeInputs.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Wybierz trening i wprowadź dane!");
        return;
    }

    QSqlDatabase db = DatabaseManager::instance().database();
    db.transaction();

    QSqlQuery q(db);
    // 1. Nowa sesja
    q.prepare("INSERT INTO workout_sessions (user_id, workout_id, date) VALUES (:uid, :wid, :date)");
    q.bindValue(":uid", currentUser->getId());
    q.bindValue(":wid", selectedWorkoutId);
    q.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));

    if (!q.exec()) { db.rollback(); return; }
    int sessionId = q.lastInsertId().toInt();

    // 2. Zapis wyników dla każdego ćwiczenia z formularza
    for (const auto& input : activeInputs) {
        q.prepare("INSERT INTO exercise_results (session_id, exercise_id, difficulty) VALUES (:sid, :eid, 3)");
        q.bindValue(":sid", sessionId);
        q.bindValue(":eid", input.exerciseId);
        q.exec();
        int resId = q.lastInsertId().toInt();

        q.prepare("INSERT INTO strength_results (result_id, weight, reps) VALUES (:rid, :w, :r)");
        q.bindValue(":rid", resId);
        q.bindValue(":w", input.weightInput->text().toDouble());
        q.bindValue(":r", input.repsInput->text().toInt());
        q.exec();
    }

    if (db.commit()) {
        QMessageBox::information(this, "Sukces", "Trening zapisany w historii!");
        clearForm();
        selectedWorkoutId = -1;
    }
}
