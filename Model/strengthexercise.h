#ifndef STRENGTHEXERCISE_H
#define STRENGTHEXERCISE_H

#include "Exercise.h"

class StrengthExercise : public Exercise {
public:
    StrengthExercise();
    StrengthExercise(int id, int workoutId, const QString& name);

    // domyślny typ = Strength
};

#endif // STRENGTHEXERCISE_H
