#include <QCoreApplication>
#include <QDebug>

// Database
#include "Database/databasemanager.h"

// Utils
#include "Utils/passwordhasher.h"

// Repositories
#include "Repository/userrepository.h"
#include "Repository/workoutrepository.h"
#include "Repository/exerciserepository.h"
#include "Repository/workoutsessionrepository.h"
#include "Repository/exerciseresultrepository.h"
#include "Repository/historyrepository.h"
#include "Repository/exercisehistoryrepository.h"

// Model
#include "Model/trainingmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "=== START BACKEND TEST ===";

    // 1️⃣ Połączenie z bazą
    if (!DatabaseManager::connect()) {
        qDebug() << "DB connection failed!";
        return -1;
    }

    // 2️⃣ Utworzenie użytkownika (jeśli nie istnieje)
    QString login = "testuser";
    QString passwordHash = PasswordHasher::hash("1234");

    if (!UserRepository::loginValid(login, passwordHash)) {
        UserModel u;
        u.login = login;
        u.passwordHash = passwordHash;
        u.birthDate = QDate(2000, 1, 1);
        u.heightCm = 180;
        u.weightKg = 80.0;

        UserRepository::createUser(u);
        qDebug() << "User created";
    }

    auto userOpt = UserRepository::getByLogin(login);
    if (!userOpt.has_value()) {
        qDebug() << "User fetch failed!";
        return -1;
    }
    int userId = userOpt->id;

    // 3️⃣ Utworzenie treningu
    int workoutId = WorkoutRepository::create("Push Day");
    qDebug() << "Workout created, id =" << workoutId;

    // 4️⃣ Dodanie ćwiczeń
    int benchId = ExerciseRepository::create(
        workoutId, "Bench Press", ExerciseType::Strength
        );
    int runId = ExerciseRepository::create(
        workoutId, "Treadmill Run", ExerciseType::Cardio
        );

    qDebug() << "Exercises created:" << benchId << runId;

    // 5️⃣ Start treningu
    TrainingManager tm;
    tm.startWorkout(userId, workoutId);

    // 6️⃣ Zapis wyników
    tm.saveStrengthExercise(benchId, 80.0, 8, 7);
    tm.saveCardioExercise(runId, 2.5, 6);

    tm.finishWorkout();

    // 7️⃣ Odczyt historii treningów
    qDebug() << "\n=== WORKOUT HISTORY ===";
    auto history = HistoryRepository::getWorkoutHistory(userId);

    for (const auto& h : history) {
        qDebug() << h.date << h.workoutName
                 << "(sessionId =" << h.sessionId << ")";
    }

    // 8️⃣ Odczyt ćwiczeń z ostatniej sesji
    if (!history.empty()) {
        int lastSessionId = history.front().sessionId;

        qDebug() << "\n=== EXERCISES FOR SESSION" << lastSessionId << "===";
        auto exercises =
            ExerciseHistoryRepository::getForSession(lastSessionId);

        for (const auto& e : exercises) {
            qDebug()
            << e.exerciseName
            << "[" << e.type << "]"
            << "weight:" << e.weight
            << "reps:" << e.reps
            << "distance:" << e.distance
            << "difficulty:" << e.difficulty;
        }
    }

    qDebug() << "\n=== BACKEND TEST FINISHED ===";

    return 0;
}
