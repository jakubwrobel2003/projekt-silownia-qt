#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <QStringList>
#include <QList>

#include "./Database/DatabaseManager.h"

// ===============================
// STRUKTURA PLANU TRENINGU
// ===============================
struct WorkoutExercisePlan
{
    int exerciseDefId;     // id z exercise_definitions
    QString type;          // "strength" / "cardio"

    // strength
    int sets = 0;
    int reps = 0;
    double weight = 0.0;

    // cardio
    int duration = 0;
    double distance = 0.0;
};

class TrainingManager
{
public:
    explicit TrainingManager(DatabaseManager& db);

    // ===== LISTA ĆWICZEŃ =====
    QStringList getExerciseNamesByType(const QString& type);
    int getExerciseDefinitionId(const QString& name);

    // ===== ZAPIS TRENINGU =====
    bool saveWorkoutWithPlan(
        const QString& workoutName,
        const QList<WorkoutExercisePlan>& plan
        );

private:
    DatabaseManager& db;
};

#endif // TRAININGMANAGER_H
