#ifndef AIWORKOUTPLAN_H
#define AIWORKOUTPLAN_H

#include <QString>
#include <QList>

// ===============================
// JEDNO ĆWICZENIE Z AI
// ===============================
struct AIExercise
{
    QString name;      // musi istnieć w exercise_definitions
    QString type;      // "strength" / "cardio"

    // strength
    int sets = 0;
    int reps = 0;
    double weight = 0.0;

    // cardio
    int duration = 0;
    double distance = 0.0;
};

// ===============================
// PLAN TRENINGU Z AI
// ===============================
struct AIWorkoutPlan
{
    QString name;                  // nazwa treningu
    QList<AIExercise> exercises;   // LISTA ĆWICZEŃ
};

#endif // AIWORKOUTPLAN_H
