#ifndef WORKOUTSESSION_H
#define WORKOUTSESSION_H

#include <QDate>

class WorkoutSession {
public:
    WorkoutSession();
    WorkoutSession(int id, int userId, int workoutId, const QDate& date);

    int getId() const;
    int getUserId() const;
    int getWorkoutId() const;
    QDate getDate() const;

    void setId(int id);
    void setUserId(int userId);
    void setWorkoutId(int workoutId);
    void setDate(const QDate& date);

private:
    int id;
    int userId;
    int workoutId;
    QDate date;
};

#endif // WORKOUTSESSION_H
