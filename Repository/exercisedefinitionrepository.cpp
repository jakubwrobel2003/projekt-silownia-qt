#include "ExerciseDefinitionRepository.h"
#include <QSqlQuery>

std::vector<ExerciseDefinitionDTO> ExerciseDefinitionRepository::getAll() {
    std::vector<ExerciseDefinitionDTO> list;
    QSqlQuery q("SELECT * FROM exercise_definitions ORDER BY name");

    while (q.next()) {
        list.push_back({
            q.value("id").toInt(),
            q.value("name").toString(),
            q.value("type").toString()
        });
    }
    return list;
}

ExerciseDefinitionDTO ExerciseDefinitionRepository::getById(int id) {
    QSqlQuery q;
    q.prepare("SELECT * FROM exercise_definitions WHERE id = ?");
    q.addBindValue(id);
    q.exec();
    q.next();

    return {
        q.value("id").toInt(),
        q.value("name").toString(),
        q.value("type").toString()
    };
}
