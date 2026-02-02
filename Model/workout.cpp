#include "workout.h"

void Workout::setName(const QString &name)
{
    this->name = name;
}

void Workout::addExercise(std::shared_ptr<Exercise> exercise)
{
    exercises.push_back(exercise);
}

void Workout::removeExercise(int index)
{
    if (index < 0 || index >= static_cast<int>(exercises.size()))
        return;

    exercises.erase(exercises.begin() + index);
}

std::shared_ptr<Exercise> Workout::getExercise(int index) const
{
    if (index < 0 || index >= static_cast<int>(exercises.size()))
        return nullptr;

    return exercises[index];
}

void Workout::updateExercise(int index, std::shared_ptr<Exercise> exercise)
{
    if (index < 0 || index >= static_cast<int>(exercises.size()))
        return;

    exercises[index] = exercise;
}

int Workout::getExerciseCount() const
{
    return static_cast<int>(exercises.size());
}
void Workout::clearExercises()
{
    exercises.clear();
}
