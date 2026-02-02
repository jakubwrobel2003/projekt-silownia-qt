#ifndef WORKOUT_H
#define WORKOUT_H

#include <QString>
#include <vector>
#include <memory>
#include "exercise.h"

class Workout
{
public:
    void setName(const QString& name);

    void addExercise(std::shared_ptr<Exercise> exercise);
    void removeExercise(int index);

    std::shared_ptr<Exercise> getExercise(int index) const;
    void updateExercise(int index, std::shared_ptr<Exercise> exercise);

    int getExerciseCount() const;
    void clearExercises();
private:
    QString name;
    std::vector<std::shared_ptr<Exercise>> exercises;
};

#endif // WORKOUT_H
