#ifndef CARDIOEXERCISE_H
#define CARDIOEXERCISE_H

#include "Exercise.h"

class CardioExercise : public Exercise {
public:
    CardioExercise();
    CardioExercise(int id, int workoutId, const QString& name);

    // domyślny typ = Cardio
};

#endif // CARDIOEXERCISE_H
