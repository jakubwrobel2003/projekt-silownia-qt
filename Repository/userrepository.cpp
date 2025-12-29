#include "UserRepository.h"
#include <QSqlQuery>
#include <QVariant>

bool UserRepository::createUser(const UserModel& u) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO users (login, password_hash, birth_date, height_cm, weight_kg) "
        "VALUES (?, ?, ?, ?, ?)"
        );
    q.addBindValue(u.login);
    q.addBindValue(u.passwordHash);
    q.addBindValue(u.birthDate.toString(Qt::ISODate));
    q.addBindValue(u.heightCm);
    q.addBindValue(u.weightKg);

    return q.exec();
}

std::optional<UserModel> UserRepository::getByLogin(const QString& login) {
    QSqlQuery q;
    q.prepare("SELECT * FROM users WHERE login = ?");
    q.addBindValue(login);

    if (!q.exec() || !q.next())
        return std::nullopt;

    UserModel u;
    u.id = q.value("id").toInt();
    u.login = q.value("login").toString();
    u.passwordHash = q.value("password_hash").toString();
    u.birthDate = QDate::fromString(q.value("birth_date").toString(), Qt::ISODate);
    u.heightCm = q.value("height_cm").toInt();
    u.weightKg = q.value("weight_kg").toDouble();

    return u;
}

bool UserRepository::loginValid(const QString& login, const QString& passwordHash) {
    QSqlQuery q;
    q.prepare(
        "SELECT id FROM users WHERE login = ? AND password_hash = ?"
        );
    q.addBindValue(login);
    q.addBindValue(passwordHash);

    return q.exec() && q.next();
}
