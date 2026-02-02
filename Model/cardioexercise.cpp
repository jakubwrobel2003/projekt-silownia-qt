#include "cardioexercise.h"

CardioExercise::CardioExercise()
    : Exercise(), durationMinutes(0), distanceKm(0.0)
{
    type = ExerciseType::Cardio;
}

CardioExercise::CardioExercise(const QString& name,
                               int durationMinutes,
                               double distanceKm)
    : Exercise(-1, -1, name, ExerciseType::Cardio),
    durationMinutes(durationMinutes),
    distanceKm(distanceKm)
{
}

int CardioExercise::getDuration() const
{
    return durationMinutes;
}

double CardioExercise::getDistance() const
{
    return distanceKm;
}

void CardioExercise::setDuration(int minutes)
{
    durationMinutes = minutes;
}

void CardioExercise::setDistance(double km)
{
    distanceKm = km;
}
