#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QDate>
#include "workout.h"
class UserModel {
public:
    // Konstruktor
    UserModel(int id = 0, const QString& login = "", const QString& passwordHash = "",
              const QDate& birthDate = QDate(), int heightCm = 0, double weightKg = 0.0);

    // Gettery
    int getId() const;
    QString getLogin() const;
    QString getPasswordHash() const;
    QDate getBirthDate() const;
    int getHeightCm() const;
    double getWeightKg() const;

    // Settery
    void setId(int newId);
    void setLogin(const QString& newLogin);
    void setPasswordHash(const QString& newPasswordHash);
    void setBirthDate(const QDate& newBirthDate);
    void setHeightCm(int newHeightCm);
    void setWeightKg(double newWeightKg);
    void addWorkout(const Workout& workout);
    const std::vector<Workout>& getWorkouts() const;
private:
    int id;
    QString login;
    QString passwordHash;
    QDate birthDate;
    int heightCm;
    double weightKg;
    std::vector<Workout> workouts;
};

#endif // USERMODEL_H
