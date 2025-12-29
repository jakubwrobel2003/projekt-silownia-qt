#include "WorkoutSession.h"

WorkoutSession::WorkoutSession()
    : id(-1), userId(-1), workoutId(-1), date(QDate::currentDate()) {}

WorkoutSession::WorkoutSession(int id, int userId, int workoutId, const QDate& date)
    : id(id), userId(userId), workoutId(workoutId), date(date) {}

int WorkoutSession::getId() const {
    return id;
}

int WorkoutSession::getUserId() const {
    return userId;
}

int WorkoutSession::getWorkoutId() const {
    return workoutId;
}

QDate WorkoutSession::getDate() const {
    return date;
}

void WorkoutSession::setId(int newId) {
    id = newId;
}

void WorkoutSession::setUserId(int newUserId) {
    userId = newUserId;
}

void WorkoutSession::setWorkoutId(int newWorkoutId) {
    workoutId = newWorkoutId;
}

void WorkoutSession::setDate(const QDate& newDate) {
    date = newDate;
}
