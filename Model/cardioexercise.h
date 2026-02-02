#ifndef CARDIOEXERCISE_H
#define CARDIOEXERCISE_H

#include "exercise.h"

class CardioExercise : public Exercise
{
public:
    CardioExercise();
    CardioExercise(const QString& name,
                   int durationMinutes,
                   double distanceKm);

    // GETTERY
    int getDuration() const;
    double getDistance() const;

    // SETTERY
    void setDuration(int minutes);
    void setDistance(double km);

private:
    int durationMinutes;
    double distanceKm;
};

#endif // CARDIOEXERCISE_H
