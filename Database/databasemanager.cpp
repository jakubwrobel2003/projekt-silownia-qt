#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ===============================
// SINGLETON
// ===============================
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

// ===============================
// ZWRÓCENIE BAZY
// ===============================
QSqlDatabase DatabaseManager::database()
{
    return QSqlDatabase::database();
}

// ===============================
// POŁĄCZENIE Z BAZĄ
// ===============================
bool DatabaseManager::connect()
{
    if (QSqlDatabase::contains())
        return true;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // 🔒 ZOSTAWIAMY STARĄ NAZWĘ BAZY
    db.setDatabaseName("silownia.db");

    if (!db.open())
    {
        qCritical() << "DB ERROR:" << db.lastError().text();
        return false;
    }

    initDatabase();
    qDebug() << "Database connected successfully";
    return true;
}

// ===============================
// INICJALIZACJA STRUKTURY BAZY
// ===============================
void DatabaseManager::initDatabase()
{
    QSqlQuery q;

    // ===== USERS (LOGOWANIE) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "login TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "birth_date TEXT NOT NULL,"
        "height_cm INTEGER NOT NULL,"
        "weight_kg REAL NOT NULL,"
        "created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP)"
        );

    // ===== WORKOUTS (SZABLONY TRENINGÓW) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS workouts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL)"
        );

    // ===== EXERCISES (PRZYPISANIE DO TRENINGU) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS exercises ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "workout_id INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "type TEXT NOT NULL CHECK(type IN ('strength','cardio')),"
        "FOREIGN KEY(workout_id) REFERENCES workouts(id) ON DELETE CASCADE)"
        );

    // ===== WORKOUT SESSIONS =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS workout_sessions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "workout_id INTEGER NOT NULL,"
        "date TEXT NOT NULL,"
        "FOREIGN KEY(user_id) REFERENCES users(id),"
        "FOREIGN KEY(workout_id) REFERENCES workouts(id))"
        );

    // ===== EXERCISE RESULTS =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS exercise_results ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "session_id INTEGER NOT NULL,"
        "exercise_id INTEGER NOT NULL,"
        "difficulty INTEGER NOT NULL,"
        "FOREIGN KEY(session_id) REFERENCES workout_sessions(id),"
        "FOREIGN KEY(exercise_id) REFERENCES exercises(id))"
        );

    // ===== STRENGTH RESULTS =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS strength_results ("
        "result_id INTEGER PRIMARY KEY,"
        "weight REAL NOT NULL,"
        "reps INTEGER NOT NULL,"
        "FOREIGN KEY(result_id) REFERENCES exercise_results(id) ON DELETE CASCADE)"
        );

    // ===== CARDIO RESULTS =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS cardio_results ("
        "result_id INTEGER PRIMARY KEY,"
        "distance REAL NOT NULL,"
        "FOREIGN KEY(result_id) REFERENCES exercise_results(id) ON DELETE CASCADE)"
        );

    // ===== DEFINICJE ĆWICZEŃ (LISTA) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS exercise_definitions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE NOT NULL,"
        "type TEXT NOT NULL CHECK(type IN ('strength','cardio')))"
        );

    // ===== PLAN TRENINGU (NOWA, KLUCZOWA TABELA) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS workout_exercise_plan ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "workout_id INTEGER NOT NULL,"
        "exercise_def_id INTEGER NOT NULL,"
        "type TEXT NOT NULL CHECK(type IN ('strength','cardio')),"
        "sets INTEGER,"
        "reps INTEGER,"
        "weight REAL,"
        "duration INTEGER,"
        "distance REAL,"
        "FOREIGN KEY(workout_id) REFERENCES workouts(id) ON DELETE CASCADE,"
        "FOREIGN KEY(exercise_def_id) REFERENCES exercise_definitions(id))"
        );

    // ===== DANE STARTOWE =====
    QSqlQuery check;
    check.exec("SELECT COUNT(*) FROM exercise_definitions");
    check.next();

    if (check.value(0).toInt() == 0)
    {
        // --- STRENGTH ---
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Bench Press','strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Squat','strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Deadlift','strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Overhead Press','strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Pull Ups','strength')");

        // --- CARDIO ---
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Treadmill Run','cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Cycling','cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Rowing Machine','cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Jump Rope','cardio')");
    }

    // ===== PREDEFINED USER (TEST) =====
    QSqlQuery checkUser;
    checkUser.exec("SELECT COUNT(*) FROM users WHERE login = 'wojfer87'");
    checkUser.next();

    if (checkUser.value(0).toInt() == 0)
    {
        QSqlQuery insertUser;
        insertUser.prepare(
            "INSERT INTO users (login, password_hash, birth_date, height_cm, weight_kg) "
            "VALUES (:login, :password, :birth, :height, :weight)"
            );

        insertUser.bindValue(":login", "wojfer87");
        insertUser.bindValue(":password", "812137"); // demo
        insertUser.bindValue(":birth", "1990-01-01");
        insertUser.bindValue(":height", 180);
        insertUser.bindValue(":weight", 75.0);

        insertUser.exec();
    }
    // ===== FAKOWE DANE TRENINGOWE (TESTY WYKRESÓW) =====
    QSqlQuery checkSessions;
    checkSessions.exec("SELECT COUNT(*) FROM workout_sessions WHERE user_id = 1");
    checkSessions.next();

    if (checkSessions.value(0).toInt() == 0) {
        qDebug() << "Generating fake workout data...";

        // 1. Najpierw upewniamy się, że mamy zdefiniowany jakiś trening w tabeli 'workouts'
        q.exec("INSERT INTO workouts (name) VALUES ('FBW - Full Body Workout')");
        int workoutId = q.lastInsertId().toInt();

        // 2. Pobieramy ID ćwiczenia (np. Bench Press)
        q.exec("SELECT id FROM exercise_definitions WHERE name = 'Bench Press'");
        q.next();
        int benchPressDefId = q.value(0).toInt();

        // 3. Dodajemy sesje: 3 treningi w tygodniu przez ostatnie 2 tygodnie
        // Daty: 2026-01-19, 21, 23 (Pn, Śr, Pt) oraz 2026-01-26, 28, 30
        QStringList dates = {
            "2026-01-19", "2026-01-21", "2026-01-23",
            "2026-01-26", "2026-01-28", "2026-01-30"
        };

        double weight = 40.0; // Startowy ciężar

        for (const QString& date : dates) {
            // A. Dodajemy sesję treningową
            QSqlQuery insertSession;
            insertSession.prepare("INSERT INTO workout_sessions (user_id, workout_id, date) VALUES (1, :wid, :date)");
            insertSession.bindValue(":wid", workoutId);
            insertSession.bindValue(":date", date);
            insertSession.exec();
            int sessionId = insertSession.lastInsertId().toInt();

            // B. Dodajemy wynik ćwiczenia (Exercise Result)
            QSqlQuery insertRes;
            insertRes.prepare("INSERT INTO exercise_results (session_id, exercise_id, difficulty) VALUES (:sid, :eid, 3)");
            insertRes.bindValue(":sid", sessionId);
            insertRes.bindValue(":eid", benchPressDefId); // Używamy ID z definicji
            insertRes.exec();
            int resId = insertRes.lastInsertId().toInt();

            // C. Dodajemy konkretne dane siłowe (Strength Result) - progresja ciężaru
            QSqlQuery insertStrength;
            insertStrength.prepare("INSERT INTO strength_results (result_id, weight, reps) VALUES (:rid, :weight, 10)");
            insertStrength.bindValue(":rid", resId);
            insertStrength.bindValue(":weight", weight);
            insertStrength.exec();

            weight += 2.5; // Dodajemy ciężaru co trening, żeby wykres ładnie wyglądał
        }
        qDebug() << "Fake data generated successfully.";
    }
    qDebug() << "Database initialized (safe mode)";
}
