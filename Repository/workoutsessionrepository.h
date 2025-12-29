#ifndef WORKOUTSESSIONREPOSITORY_H
#define WORKOUTSESSIONREPOSITORY_H
#include <QString>
class WorkoutSessionRepository {
public:
    static int startSession(int userId, int workoutId, const QString& date);
};

#endif // WORKOUTSESSIONREPOSITORY_H
