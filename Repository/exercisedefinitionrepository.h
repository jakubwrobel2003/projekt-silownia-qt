#ifndef EXERCISEDEFINITIONREPOSITORY_H
#define EXERCISEDEFINITIONREPOSITORY_H

#include <QString>
#include <vector>

struct ExerciseDefinitionDTO {
    int id;
    QString name;
    QString type;
};

class ExerciseDefinitionRepository {
public:
    static std::vector<ExerciseDefinitionDTO> getAll();
    static ExerciseDefinitionDTO getById(int id);
};

#endif
