#include "ExerciseRepository.h"
#include <QSqlQuery>
#include <QVariant>

int ExerciseRepository::create(int workoutId, const QString& name, ExerciseType type) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO exercises (workout_id, name, type) "
        "VALUES (?, ?, ?)"
        );
    q.addBindValue(workoutId);
    q.addBindValue(name);
    q.addBindValue(Exercise::typeToString(type));

    q.exec();
    return q.lastInsertId().toInt();
}

std::vector<Exercise> ExerciseRepository::getForWorkout(int workoutId) {
    std::vector<Exercise> result;

    QSqlQuery q;
    q.prepare(
        "SELECT * FROM exercises WHERE workout_id = ?"
        );
    q.addBindValue(workoutId);
    q.exec();

    while (q.next()) {
        Exercise e(
            q.value("id").toInt(),
            q.value("workout_id").toInt(),
            q.value("name").toString(),
            Exercise::stringToType(q.value("type").toString())
            );
        result.push_back(e);
    }

    return result;
}
