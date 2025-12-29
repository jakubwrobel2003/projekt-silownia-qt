#include "WorkoutRepository.h"
#include <QSqlQuery>
#include <QVariant>

int WorkoutRepository::create(const QString& name) {
    QSqlQuery q;
    q.prepare("INSERT INTO workouts (name) VALUES (?)");
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId().toInt();
}

std::vector<WorkoutDTO> WorkoutRepository::getAll() {
    std::vector<WorkoutDTO> list;
    QSqlQuery q("SELECT * FROM workouts");

    while (q.next()) {
        list.push_back({
            q.value("id").toInt(),
            q.value("name").toString()
        });
    }
    return list;
}
