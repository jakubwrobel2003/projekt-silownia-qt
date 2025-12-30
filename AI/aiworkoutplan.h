#ifndef AIWORKOUTPLAN_H
#define AIWORKOUTPLAN_H

#include <QString>
#include <QList>

struct AIWorkoutExercise {
    int exerciseDefinitionId;   // FK -> exercise_definitions.id
    int sets;
    QString reps;
};

struct AIWorkoutPlan {
    QString workoutName;                // -> workouts.name
    QList<AIWorkoutExercise> exercises; // -> exercises
};

#endif // AIWORKOUTPLAN_H
