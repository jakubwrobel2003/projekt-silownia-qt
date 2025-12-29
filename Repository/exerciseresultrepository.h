#ifndef EXERCISERESULTREPOSITORY_H
#define EXERCISERESULTREPOSITORY_H



class ExerciseResultRepository {
public:
    static int createBaseResult(int sessionId, int exerciseId, int difficulty);
    static void saveStrengthResult(int resultId, double weight, int reps);
    static void saveCardioResult(int resultId, double distance);
};


#endif // EXERCISERESULTREPOSITORY_H
