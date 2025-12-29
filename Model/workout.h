#ifndef WORKOUT_H
#define WORKOUT_H

#include <QString>
#include <vector>
#include <memory>
#include "Exercise.h"

class Workout {
public:
    Workout();
    Workout(int id, const QString& name);

    int getId() const;
    QString getName() const;
    const std::vector<std::shared_ptr<Exercise>>& getExercises() const;

    void setId(int id);
    void setName(const QString& name);

    void addExercise(const std::shared_ptr<Exercise>& exercise);

private:
    int id;
    QString name;
    std::vector<std::shared_ptr<Exercise>> exercises;
};

#endif // WORKOUT_H
