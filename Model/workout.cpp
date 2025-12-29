#include "Workout.h"

Workout::Workout()
    : id(-1) {}

Workout::Workout(int id, const QString& name)
    : id(id), name(name) {}

int Workout::getId() const {
    return id;
}

QString Workout::getName() const {
    return name;
}

const std::vector<std::shared_ptr<Exercise>>& Workout::getExercises() const {
    return exercises;
}

void Workout::setId(int newId) {
    id = newId;
}

void Workout::setName(const QString& newName) {
    name = newName;
}

void Workout::addExercise(const std::shared_ptr<Exercise>& exercise) {
    exercises.push_back(exercise);
}
