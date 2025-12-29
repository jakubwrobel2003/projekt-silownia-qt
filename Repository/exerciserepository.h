#ifndef EXERCISEREPOSITORY_H
#define EXERCISEREPOSITORY_H

#include <QString>
#include <vector>
#include "Model/Exercise.h"

class ExerciseRepository {
public:
    // dodanie ćwiczenia do treningu
    static int create(int workoutId, const QString& name, ExerciseType type);

    // pobranie wszystkich ćwiczeń dla danego treningu
    static std::vector<Exercise> getForWorkout(int workoutId);
};

#endif // EXERCISEREPOSITORY_H
