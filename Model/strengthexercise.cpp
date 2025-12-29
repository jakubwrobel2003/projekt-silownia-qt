#include "StrengthExercise.h"

StrengthExercise::StrengthExercise()
    : Exercise() {
    type = ExerciseType::Strength;
}

StrengthExercise::StrengthExercise(int id, int workoutId, const QString& name)
    : Exercise(id, workoutId, name, ExerciseType::Strength) {}
