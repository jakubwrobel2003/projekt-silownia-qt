#include "strengthexercise.h"

StrengthExercise::StrengthExercise()
    : Exercise(), sets(0), reps(0), weight(0.0)
{
    type = ExerciseType::Strength;
}

StrengthExercise::StrengthExercise(const QString& name,
                                   int sets,
                                   int reps,
                                   double weight)
    : Exercise(-1, -1, name, ExerciseType::Strength),
    sets(sets),
    reps(reps),
    weight(weight)
{
}

int StrengthExercise::getSets() const
{
    return sets;
}

int StrengthExercise::getReps() const
{
    return reps;
}

double StrengthExercise::getWeight() const
{
    return weight;
}

void StrengthExercise::setSets(int sets)
{
    this->sets = sets;
}

void StrengthExercise::setReps(int reps)
{
    this->reps = reps;
}

void StrengthExercise::setWeight(double weight)
{
    this->weight = weight;
}
