#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QDate>

class UserModel {
public:
    int id;
    QString login;
    QString passwordHash;
    QDate birthDate;
    int heightCm;
    double weightKg;
};

#endif // USERMODEL_H
