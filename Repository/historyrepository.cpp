#include "HistoryRepository.h"
#include <QSqlQuery>

std::vector<WorkoutHistoryItem> HistoryRepository::getWorkoutHistory(int userId)
{
    std::vector<WorkoutHistoryItem> history;

    QSqlQuery q;
    q.prepare(
        "SELECT ws.id AS session_id, ws.date, w.name AS workout_name "
        "FROM workout_sessions ws "
        "JOIN workouts w ON w.id = ws.workout_id "
        "WHERE ws.user_id = ? "
        "ORDER BY ws.date DESC"
        );
    q.addBindValue(userId);
    q.exec();

    while (q.next()) {
        WorkoutHistoryItem item;
        item.sessionId = q.value("session_id").toInt();
        item.date = q.value("date").toString();
        item.workoutName = q.value("workout_name").toString();
        history.push_back(item);
    }

    return history;
}
