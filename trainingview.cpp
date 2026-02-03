#include "trainingview.h"
#include "ui_trainingview.h"
#include "./Database/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QSpinBox>
#include <QDoubleSpinBox>

trainingview::trainingview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::trainingview)
{
    ui->setupUi(this);
    trainingManager = new TrainingManager(DatabaseManager::instance());

    ui->dateEdit->setDate(QDate::currentDate());

    // Konfiguracja tabeli
    ui->tableDetails->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableDetails->verticalHeader()->setDefaultSectionSize(50); // Wyższe wiersze dla wygody
}

trainingview::~trainingview()
{
    delete ui;
    delete trainingManager;
}

void trainingview::setUserData(UserModel *user)
{
    currentUser = user;
    refreshWorkoutList();
}

void trainingview::refreshWorkoutList()
{
    ui->listWorkouts->clear();
    ui->tableDetails->setRowCount(0);

    QSqlQuery q(DatabaseManager::instance().database());
    q.exec("SELECT id, name FROM workouts");

    while (q.next()) {
        QListWidgetItem *item = new QListWidgetItem(q.value(1).toString());
        item->setData(Qt::UserRole, q.value(0).toInt());
        ui->listWorkouts->addItem(item);
    }
}

void trainingview::on_listWorkouts_itemClicked()
{
    QListWidgetItem *item = ui->listWorkouts->currentItem();
    if (!item) return;

    selectedWorkoutId = item->data(Qt::UserRole).toInt();

    // Czyścimy tabelę
    ui->tableDetails->setRowCount(0);

    // Pobieramy plan z bazy
    QList<TrainingPlanDetail> details = trainingManager->getWorkoutDetails(selectedWorkoutId);

    for (const auto& det : details) {
        int row = ui->tableDetails->rowCount();
        ui->tableDetails->insertRow(row);

        // 1. Nazwa Ćwiczenia (Przechowujemy też ID ćwiczenia w tym itemie)
        QTableWidgetItem *nameItem = new QTableWidgetItem(det.exerciseName);
        nameItem->setData(Qt::UserRole, det.exerciseDefId); // WAŻNE: Tu chowamy ID do zapisu
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable); // Tylko do odczytu
        ui->tableDetails->setItem(row, 0, nameItem);

        // 2. Typ (Siłowe/Cardio)
        QString typePL = (det.type == "strength") ? "Siłowe" : "Cardio";
        QTableWidgetItem *typeItem = new QTableWidgetItem(typePL);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        ui->tableDetails->setItem(row, 1, typeItem);

        // 3, 4, 5. Kolumny edycyjne (Widgety)

        if (det.type == "strength") {
            // SERIE
            QSpinBox *sbSets = new QSpinBox();
            sbSets->setRange(0, 100);
            sbSets->setValue(det.targetSets); // Wstawiamy wartość z planu
            sbSets->setStyleSheet("background-color: #303040; color: white; border: 1px solid gray;");
            ui->tableDetails->setCellWidget(row, 2, sbSets);

            // POWTÓRZENIA
            QSpinBox *sbReps = new QSpinBox();
            sbReps->setRange(0, 500);
            sbReps->setValue(det.targetReps); // Wstawiamy wartość z planu
            sbReps->setStyleSheet("background-color: #303040; color: white; border: 1px solid gray;");
            ui->tableDetails->setCellWidget(row, 3, sbReps);

            // CIĘŻAR
            QDoubleSpinBox *sbWeight = new QDoubleSpinBox();
            sbWeight->setRange(0, 500);
            sbWeight->setValue(det.targetWeight); // Wstawiamy wartość z planu
            sbWeight->setStyleSheet("background-color: #303040; color: white; border: 1px solid gray;");
            ui->tableDetails->setCellWidget(row, 4, sbWeight);
        }
        else {
            // CARDIO

            // CZAS (min) - zamiast Serii
            QSpinBox *sbDuration = new QSpinBox();
            sbDuration->setRange(0, 999);
            sbDuration->setValue(det.targetDuration);
            sbDuration->setStyleSheet("background-color: #303040; color: white; border: 1px solid gray;");
            ui->tableDetails->setCellWidget(row, 2, sbDuration);

            // DYSTANS (km) - zamiast Powtórzeń
            QDoubleSpinBox *sbDist = new QDoubleSpinBox();
            sbDist->setRange(0, 999);
            sbDist->setValue(det.targetDistance);
            sbDist->setStyleSheet("background-color: #303040; color: white; border: 1px solid gray;");
            ui->tableDetails->setCellWidget(row, 3, sbDist);

            // CIĘŻAR - Puste dla cardio
            QTableWidgetItem *empty = new QTableWidgetItem("-");
            empty->setTextAlignment(Qt::AlignCenter);
            empty->setFlags(empty->flags() & ~Qt::ItemIsEditable);
            ui->tableDetails->setItem(row, 4, empty);
        }
    }
}

void trainingview::on_btnSaveWorkout_clicked()
{
    if (!currentUser || selectedWorkoutId == -1) {
        QMessageBox::warning(this, "Błąd", "Wybierz plan treningowy!");
        return;
    }

    if (ui->tableDetails->rowCount() == 0) return;

    QSqlDatabase db = DatabaseManager::instance().database();
    QString selectedDate = ui->dateEdit->date().toString("yyyy-MM-dd");

    db.transaction();
    QSqlQuery q(db);

    // 1. Tworzymy sesję
    q.prepare("INSERT INTO workout_sessions (user_id, workout_id, date) VALUES (:uid, :wid, :date)");
    q.bindValue(":uid", currentUser->getId());
    q.bindValue(":wid", selectedWorkoutId);
    q.bindValue(":date", selectedDate);

    if (!q.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Błąd", "Błąd zapisu sesji: " + q.lastError().text());
        return;
    }
    int sessionId = q.lastInsertId().toInt();

    // 2. Iterujemy przez wiersze tabeli i pobieramy dane z widgetów
    for (int row = 0; row < ui->tableDetails->rowCount(); ++row) {
        // Pobieramy ID ćwiczenia z ukrytych danych w kolumnie 0
        int exerciseDefId = ui->tableDetails->item(row, 0)->data(Qt::UserRole).toInt();
        QString typeStr = ui->tableDetails->item(row, 1)->text(); // "Siłowe" lub "Cardio"

        // Pobieramy widgety
        QWidget *wCol2 = ui->tableDetails->cellWidget(row, 2);
        QWidget *wCol3 = ui->tableDetails->cellWidget(row, 3);
        QWidget *wCol4 = ui->tableDetails->cellWidget(row, 4);

        // Najpierw wpis ogólny wyniku
        q.prepare("INSERT INTO exercise_results (session_id, exercise_id, difficulty) VALUES (:sid, :eid, 3)");
        q.bindValue(":sid", sessionId);
        q.bindValue(":eid", exerciseDefId);
        q.exec();
        int resId = q.lastInsertId().toInt();

        if (typeStr == "Siłowe") {
            // Rzutujemy widgety na odpowiednie typy
            QSpinBox *sbSets = qobject_cast<QSpinBox*>(wCol2);
            QSpinBox *sbReps = qobject_cast<QSpinBox*>(wCol3);
            QDoubleSpinBox *sbWeight = qobject_cast<QDoubleSpinBox*>(wCol4);

            if (sbSets && sbReps && sbWeight) {
                q.prepare("INSERT INTO strength_results (result_id, weight, reps) VALUES (:rid, :w, :r)");
                q.bindValue(":rid", resId);
                q.bindValue(":w", sbWeight->value());
                q.bindValue(":r", sbReps->value());
                // Uwaga: W Twojej bazie strength_results nie ma kolumny 'sets',
                // więc zapisujemy tylko wagę i powtórzenia (ewentualnie trzeba rozbudować bazę o sety).
                // Zakładamy, że user zrobił tyle serii ile widać.
                q.exec();
            }
        }
        else { // Cardio
            QSpinBox *sbDur = qobject_cast<QSpinBox*>(wCol2);
            QDoubleSpinBox *sbDist = qobject_cast<QDoubleSpinBox*>(wCol3);

            if (sbDist) {
                q.prepare("INSERT INTO cardio_results (result_id, distance) VALUES (:rid, :d)");
                q.bindValue(":rid", resId);
                q.bindValue(":d", sbDist->value());
                // Czas (Duration) w Twojej bazie jest w tabeli głównej workout_exercise_plan, ale w wynikach cardio_results jest tylko distance.
                // Jeśli chcesz zapisać czas, musisz dodać kolumnę duration do cardio_results w bazie.
                q.exec();
            }
        }
    }

    if (db.commit()) {
        QMessageBox::information(this, "Sukces", "Trening zapisany! Wróć do Historii, aby go zobaczyć.");
        // Opcjonalnie: czyścimy tabelę, albo zostawiamy
        ui->listWorkouts->clearSelection();
        ui->tableDetails->setRowCount(0);
    } else {
        db.rollback();
        QMessageBox::critical(this, "Błąd", "Błąd transakcji bazy danych.");
    }
}
