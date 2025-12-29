#include "ExerciseHistoryRepository.h"
#include <QSqlQuery>

std::vector<ExerciseHistoryItem>
ExerciseHistoryRepository::getForSession(int sessionId)
{
    std::vector<ExerciseHistoryItem> list;

    QSqlQuery q;
    q.prepare(
        "SELECT e.name, e.type, er.difficulty, "
        "sr.weight, sr.reps, cr.distance "
        "FROM exercise_results er "
        "JOIN exercises e ON e.id = er.exercise_id "
        "LEFT JOIN strength_results sr ON sr.result_id = er.id "
        "LEFT JOIN cardio_results cr ON cr.result_id = er.id "
        "WHERE er.session_id = ?"
        );
    q.addBindValue(sessionId);
    q.exec();

    while (q.next()) {
        ExerciseHistoryItem i;
        i.exerciseName = q.value("name").toString();
        i.type = q.value("type").toString();
        i.difficulty = q.value("difficulty").toInt();
        i.weight = q.value("weight").toDouble();
        i.reps = q.value("reps").toInt();
        i.distance = q.value("distance").toDouble();
        list.push_back(i);
    }

    return list;
}
