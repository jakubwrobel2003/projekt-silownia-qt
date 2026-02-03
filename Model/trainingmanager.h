#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

#include <QStringList>
#include <QList>
#include "./Database/DatabaseManager.h"

// ... (Twoja struktura WorkoutExercisePlan zostaje bez zmian) ...
struct WorkoutExercisePlan
{
    int exerciseDefId;
    QString type;
    int sets = 0;
    int reps = 0;
    double weight = 0.0;
    int duration = 0;
    double distance = 0.0;
};

// [NOWE] Struktura do wyświetlania w TrainingView (Plan + Nazwa)
struct TrainingPlanDetail {
    int exerciseDefId;
    QString exerciseName;
    QString type;
    int targetSets;
    int targetReps;
    double targetWeight;
    int targetDuration;
    double targetDistance;
};

class TrainingManager
{
public:
    explicit TrainingManager(DatabaseManager& db);

    // ... (Twoje stare metody zostają) ...
    QStringList getExerciseNamesByType(const QString& type);
    int getExerciseDefinitionId(const QString& name);
    bool saveWorkoutWithPlan(const QString& workoutName, const QList<WorkoutExercisePlan>& plan);
    QString getExerciseNameById(int id);

    // [NOWE] Pobieranie szczegółów planu do wykonania
    QList<TrainingPlanDetail> getWorkoutDetails(int workoutId);

private:
    DatabaseManager& db;
};

#endif // TRAININGMANAGER_H
