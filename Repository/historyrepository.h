#ifndef HISTORYREPOSITORY_H
#define HISTORYREPOSITORY_H

#include <vector>
#include "Model/WorkoutHistoryItem.h"

class HistoryRepository {
public:
    static std::vector<WorkoutHistoryItem> getWorkoutHistory(int userId);
};

#endif // HISTORYREPOSITORY_H
