#include "ExerciseResult.h"

ExerciseResult::ExerciseResult()
    : id(-1), sessionId(-1), exerciseId(-1), difficulty(0) {}

ExerciseResult::ExerciseResult(int id, int sessionId, int exerciseId, int difficulty)
    : id(id), sessionId(sessionId), exerciseId(exerciseId), difficulty(difficulty) {}

int ExerciseResult::getId() const {
    return id;
}

int ExerciseResult::getSessionId() const {
    return sessionId;
}

int ExerciseResult::getExerciseId() const {
    return exerciseId;
}

int ExerciseResult::getDifficulty() const {
    return difficulty;
}

void ExerciseResult::setId(int newId) {
    id = newId;
}

void ExerciseResult::setSessionId(int newSessionId) {
    sessionId = newSessionId;
}

void ExerciseResult::setExerciseId(int newExerciseId) {
    exerciseId = newExerciseId;
}

void ExerciseResult::setDifficulty(int newDifficulty) {
    difficulty = newDifficulty;
}
