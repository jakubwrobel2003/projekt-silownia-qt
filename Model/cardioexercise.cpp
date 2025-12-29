#include "CardioExercise.h"

CardioExercise::CardioExercise()
    : Exercise() {
    type = ExerciseType::Cardio;
}

CardioExercise::CardioExercise(int id, int workoutId, const QString& name)
    : Exercise(id, workoutId, name, ExerciseType::Cardio) {}
