#include "WorkoutSessionRepository.h"
#include <QSqlQuery>
#include <QVariant>

int WorkoutSessionRepository::startSession(int userId, int workoutId, const QString& date) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO workout_sessions (user_id, workout_id, date) "
        "VALUES (?, ?, ?)"
        );
    q.addBindValue(userId);
    q.addBindValue(workoutId);
    q.addBindValue(date);
    q.exec();
    return q.lastInsertId().toInt();
}
