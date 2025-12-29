#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

enum class ExerciseType {
    Strength,
    Cardio
};

class Exercise {
public:
    Exercise();
    Exercise(int id, int workoutId, const QString& name, ExerciseType type);
    virtual ~Exercise();

    int getId() const;
    int getWorkoutId() const;
    QString getName() const;
    ExerciseType getType() const;

    void setId(int id);
    void setWorkoutId(int workoutId);
    void setName(const QString& name);

    static QString typeToString(ExerciseType type);
    static ExerciseType stringToType(const QString& type);

protected:
    int id;
    int workoutId;
    QString name;
    ExerciseType type;
};

#endif // EXERCISE_H
