#ifndef WORKOUTREPOSITORY_H
#define WORKOUTREPOSITORY_H


#include <QString>
#include <vector>

struct WorkoutDTO {
    int id;
    QString name;
};

class WorkoutRepository {
public:
    static int create(const QString& name);
    static std::vector<WorkoutDTO> getAll();
};


#endif // WORKOUTREPOSITORY_H
