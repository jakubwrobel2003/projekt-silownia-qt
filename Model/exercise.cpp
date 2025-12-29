#include "Exercise.h"

Exercise::Exercise()
    : id(-1), workoutId(-1), type(ExerciseType::Strength) {}

Exercise::Exercise(int id, int workoutId, const QString& name, ExerciseType type)
    : id(id), workoutId(workoutId), name(name), type(type) {}

Exercise::~Exercise() {}

int Exercise::getId() const { return id; }
int Exercise::getWorkoutId() const { return workoutId; }
QString Exercise::getName() const { return name; }
ExerciseType Exercise::getType() const { return type; }

void Exercise::setId(int newId) { id = newId; }
void Exercise::setWorkoutId(int newWorkoutId) { workoutId = newWorkoutId; }
void Exercise::setName(const QString& newName) { name = newName; }

QString Exercise::typeToString(ExerciseType t) {
    if (t == ExerciseType::Cardio)
        return "cardio";
    return "strength";
}

ExerciseType Exercise::stringToType(const QString& s) {
    if (s == "cardio")
        return ExerciseType::Cardio;
    return ExerciseType::Strength;
}
