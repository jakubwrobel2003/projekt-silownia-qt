#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QSqlDatabase DatabaseManager::database() {
    return QSqlDatabase::database();
}

bool DatabaseManager::connect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("silownia.db");

    if (!db.open()) {
        qDebug() << "DB ERROR:" << db.lastError().text();
        return false;
    }

    initDatabase();
    return true;
}

void DatabaseManager::initDatabase() {
    QSqlQuery q;

    // ===== USERS =====
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

    // ===== WORKOUTS =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS workouts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL)"
        );

    // ===== EXERCISES (BAZA) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS exercises ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "workout_id INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "type TEXT NOT NULL CHECK(type IN ('strength','cardio')))"
        );

    // ===== WORKOUT SESSIONS =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS workout_sessions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "workout_id INTEGER NOT NULL,"
        "date TEXT NOT NULL)"
        );

    // ===== EXERCISE RESULTS (BAZA) =====
    q.exec(
        "CREATE TABLE IF NOT EXISTS exercise_results ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "session_id INTEGER NOT NULL,"
        "exercise_id INTEGER NOT NULL,"
        "difficulty INTEGER NOT NULL)"
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
    q.exec(
        "CREATE TABLE IF NOT EXISTS exercise_definitions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE NOT NULL,"
        "type TEXT NOT NULL CHECK(type IN ('strength','cardio')))"
        );
    QSqlQuery check;
    check.exec("SELECT COUNT(*) FROM exercise_definitions");
    check.next();

    if (check.value(0).toInt() == 0) {
        QSqlQuery q;

        // ===== STRENGTH =====
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Bench Press', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Squat', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Deadlift', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Overhead Press', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Pull Ups', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Barbell Row', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Incline Bench Press', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Dips', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Biceps Curl', 'strength')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Triceps Pushdown', 'strength')");

        // ===== CARDIO =====
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Treadmill Run', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Cycling', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Rowing Machine', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Elliptical', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Jump Rope', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Stair Climber', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Walking', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Swimming', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Ski Erg', 'cardio')");
        q.exec("INSERT INTO exercise_definitions (name, type) VALUES ('Bike Sprint', 'cardio')");
    }
}

