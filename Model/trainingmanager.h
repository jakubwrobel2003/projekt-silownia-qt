#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

class TrainingManager {
public:
    TrainingManager();

    void startWorkout(int userId, int workoutId);
    void finishWorkout();

    void saveStrengthExercise(int exerciseId, double weight, int reps, int difficulty);
    void saveCardioExercise(int exerciseId, double distance, int difficulty);

private:
    int currentSessionId;
};

#endif // TRAININGMANAGER_H
