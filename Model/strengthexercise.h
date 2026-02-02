#ifndef STRENGTHEXERCISE_H
#define STRENGTHEXERCISE_H

#include "exercise.h"

class StrengthExercise : public Exercise
{
public:
    StrengthExercise();
    StrengthExercise(const QString& name,
                     int sets,
                     int reps,
                     double weight);

    // GETTERY
    int getSets() const;
    int getReps() const;
    double getWeight() const;

    // SETTERY (przy edycji)
    void setSets(int sets);
    void setReps(int reps);
    void setWeight(double weight);

private:
    int sets;
    int reps;
    double weight;
};

#endif // STRENGTHEXERCISE_H
