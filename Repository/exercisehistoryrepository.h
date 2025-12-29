#ifndef EXERCISEHISTORYREPOSITORY_H
#define EXERCISEHISTORYREPOSITORY_H

#include <vector>
#include "Model/ExerciseHistoryItem.h"

class ExerciseHistoryRepository {
public:
    static std::vector<ExerciseHistoryItem> getForSession(int sessionId);
};

#endif // EXERCISEHISTORYREPOSITORY_H
