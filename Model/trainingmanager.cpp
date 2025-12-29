#include "trainingmanager.h"

#include "Repository/workoutsessionrepository.h"
#include "Repository/exerciseresultrepository.h"

#include <QDate>
#include <QDebug>

TrainingManager::TrainingManager()
    : currentSessionId(-1)
{
}

/**
 * Rozpoczęcie nowego treningu (tworzy rekord workout_sessions)
 */
void TrainingManager::startWorkout(int userId, int workoutId)
{
    currentSessionId = WorkoutSessionRepository::startSession(
        userId,
        workoutId,
        QDate::currentDate().toString(Qt::ISODate)
        );

    qDebug() << "Workout started, sessionId =" << currentSessionId;
}

/**
 * Zakończenie treningu
 */
void TrainingManager::finishWorkout()
{
    qDebug() << "Workout finished, sessionId =" << currentSessionId;
    currentSessionId = -1;
}

/**
 * Zapis ćwiczenia siłowego
 */
void TrainingManager::saveStrengthExercise(
    int exerciseId,
    double weight,
    int reps,
    int difficulty
    ) {
    if (currentSessionId < 0) {
        qDebug() << "No active workout session!";
        return;
    }

    int resultId = ExerciseResultRepository::createBaseResult(
        currentSessionId,
        exerciseId,
        difficulty
        );

    ExerciseResultRepository::saveStrengthResult(
        resultId,
        weight,
        reps
        );

    qDebug() << "Strength exercise saved, resultId =" << resultId;
}

/**
 * Zapis ćwiczenia cardio
 */
void TrainingManager::saveCardioExercise(
    int exerciseId,
    double distance,
    int difficulty
    ) {
    if (currentSessionId < 0) {
        qDebug() << "No active workout session!";
        return;
    }

    int resultId = ExerciseResultRepository::createBaseResult(
        currentSessionId,
        exerciseId,
        difficulty
        );

    ExerciseResultRepository::saveCardioResult(
        resultId,
        distance
        );

    qDebug() << "Cardio exercise saved, resultId =" << resultId;
}
