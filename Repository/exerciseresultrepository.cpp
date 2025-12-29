#include "ExerciseResultRepository.h"
#include <QSqlQuery>
#include <QVariant>

int ExerciseResultRepository::createBaseResult(int sessionId, int exerciseId, int difficulty) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO exercise_results (session_id, exercise_id, difficulty) "
        "VALUES (?, ?, ?)"
        );
    q.addBindValue(sessionId);
    q.addBindValue(exerciseId);
    q.addBindValue(difficulty);
    q.exec();
    return q.lastInsertId().toInt();
}

void ExerciseResultRepository::saveStrengthResult(int resultId, double weight, int reps) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO strength_results (result_id, weight, reps) "
        "VALUES (?, ?, ?)"
        );
    q.addBindValue(resultId);
    q.addBindValue(weight);
    q.addBindValue(reps);
    q.exec();
}

void ExerciseResultRepository::saveCardioResult(int resultId, double distance) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO cardio_results (result_id, distance) "
        "VALUES (?, ?)"
        );
    q.addBindValue(resultId);
    q.addBindValue(distance);
    q.exec();
}
