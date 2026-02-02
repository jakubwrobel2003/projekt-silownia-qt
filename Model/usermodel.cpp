#include "UserModel.h"

// Konstruktor
UserModel::UserModel(int id, const QString& login, const QString& passwordHash,
                     const QDate& birthDate, int heightCm, double weightKg)
    : id(id), login(login), passwordHash(passwordHash), birthDate(birthDate),
    heightCm(heightCm), weightKg(weightKg) {}

// Gettery
int UserModel::getId() const {
    return id;
}

QString UserModel::getLogin() const {
    return login;
}

QString UserModel::getPasswordHash() const {
    return passwordHash;
}

QDate UserModel::getBirthDate() const {
    return birthDate;
}

int UserModel::getHeightCm() const {
    return heightCm;
}

double UserModel::getWeightKg() const {
    return weightKg;
}

// Settery
void UserModel::setId(int newId) {
    id = newId;
}

void UserModel::setLogin(const QString& newLogin) {
    login = newLogin;
}

void UserModel::setPasswordHash(const QString& newPasswordHash) {
    passwordHash = newPasswordHash;
}

void UserModel::setBirthDate(const QDate& newBirthDate) {
    birthDate = newBirthDate;
}

void UserModel::setHeightCm(int newHeightCm) {
    heightCm = newHeightCm;
}

void UserModel::setWeightKg(double newWeightKg) {
    weightKg = newWeightKg;
}
void UserModel::addWorkout(const Workout& workout)
{
    workouts.push_back(workout);
}

const std::vector<Workout>& UserModel::getWorkouts() const
{
    return workouts;
}
