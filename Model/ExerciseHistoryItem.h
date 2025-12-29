#ifndef EXERCISEHISTORYITEM_H
#define EXERCISEHISTORYITEM_H

#include <QString>

struct ExerciseHistoryItem {
    QString exerciseName;
    QString type;      // "strength" / "cardio"
    double weight;
    int reps;
    double distance;
    int difficulty;
};

#endif // EXERCISEHISTORYITEM_H
