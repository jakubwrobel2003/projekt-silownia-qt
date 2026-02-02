#include "trainingmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ===============================
// KONSTRUKTOR
// ===============================
TrainingManager::TrainingManager(DatabaseManager& db)
    : db(db)
{
}

// ===============================
// POBIERANIE ĆWICZEŃ Z BAZY (PO TYPIE)
// ===============================
QStringList TrainingManager::getExerciseNamesByType(const QString& type)
{
    QStringList list;
    QSqlQuery q(db.database());

    q.prepare(
        "SELECT name FROM exercise_definitions "
        "WHERE type = :type "
        "ORDER BY name"
        );
    q.bindValue(":type", type);

    if (!q.exec())
    {
        qWarning() << "getExerciseNamesByType error:" << q.lastError();
        return list;
    }

    while (q.next())
        list << q.value(0).toString();

    return list;
}

// ===============================
// POBIERANIE ID ĆWICZENIA (DEF)
// ===============================
int TrainingManager::getExerciseDefinitionId(const QString& name)
{
    QSqlQuery q(db.database());

    q.prepare(
        "SELECT id FROM exercise_definitions WHERE name = :name"
        );
    q.bindValue(":name", name);

    if (!q.exec() || !q.next())
    {
        qWarning() << "getExerciseDefinitionId error for:" << name;
        return -1;
    }

    return q.value(0).toInt();
}

// ===============================
// ZAPIS TRENINGU + PLANU
// ===============================
bool TrainingManager::saveWorkoutWithPlan(
    const QString& workoutName,
    const QList<WorkoutExercisePlan>& plan)
{
    QSqlQuery q(db.database());

    // ===============================
    // 1️⃣ ZAPIS TRENINGU
    // ===============================
    q.prepare(
        "INSERT INTO workouts (name) VALUES (:name)"
        );
    q.bindValue(":name", workoutName);

    if (!q.exec())
    {
        qCritical() << "Workout insert error:" << q.lastError();
        return false;
    }

    int workoutId = q.lastInsertId().toInt();

    // ===============================
    // 2️⃣ ZAPIS PLANU TRENINGU
    // ===============================
    for (const auto& ex : plan)
    {
        q.prepare(
            "INSERT INTO workout_exercise_plan "
            "(workout_id, exercise_def_id, type, sets, reps, weight, duration, distance) "
            "VALUES "
            "(:workoutId, :defId, :type, :sets, :reps, :weight, :duration, :distance)"
            );

        q.bindValue(":workoutId", workoutId);
        q.bindValue(":defId", ex.exerciseDefId);
        q.bindValue(":type", ex.type);
        q.bindValue(":sets", ex.sets);
        q.bindValue(":reps", ex.reps);
        q.bindValue(":weight", ex.weight);
        q.bindValue(":duration", ex.duration);
        q.bindValue(":distance", ex.distance);

        if (!q.exec())
        {
            qCritical() << "Workout plan insert error:" << q.lastError();
            return false;
        }
    }

    qDebug() << "Workout + plan saved successfully";
    return true;
}
