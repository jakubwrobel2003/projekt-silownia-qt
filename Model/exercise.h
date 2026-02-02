#ifndef EXERCISE_H
#define EXERCISE_H

#include <QString>

enum class ExerciseType
{
    Strength,
    Cardio
};

class Exercise
{
public:
    Exercise();
    Exercise(int id, int workoutId, const QString& name, ExerciseType type);
    virtual ~Exercise();

    int getId() const;
    int getWorkoutId() const;
    QString getName() const;
    ExerciseType getType() const;

    void setId(int newId);
    void setWorkoutId(int newWorkoutId);
    void setName(const QString& newName);

    static QString typeToString(ExerciseType t);
    static ExerciseType stringToType(const QString& s);

protected:
    int id;
    int workoutId;
    QString name;
    ExerciseType type;
};


#endif // EXERCISE_H
