#ifndef EXERCISERESULT_H
#define EXERCISERESULT_H

class ExerciseResult {
public:
    ExerciseResult();
    ExerciseResult(int id, int sessionId, int exerciseId, int difficulty);

    int getId() const;
    int getSessionId() const;
    int getExerciseId() const;
    int getDifficulty() const;

    void setId(int id);
    void setSessionId(int sessionId);
    void setExerciseId(int exerciseId);
    void setDifficulty(int difficulty);

private:
    int id;
    int sessionId;
    int exerciseId;
    int difficulty;
};

#endif // EXERCISERESULT_H
