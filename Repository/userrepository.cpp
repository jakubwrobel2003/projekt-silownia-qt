#include "UserRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include "./Model/usermodel.h"
bool UserRepository::createUser(const UserModel& u) {
    QSqlQuery q;
    q.prepare(
        "INSERT INTO users (login, password_hash, birth_date, height_cm, weight_kg) "
        "VALUES (?, ?, ?, ?, ?)"
        );
    q.addBindValue(u.getLogin());
    q.addBindValue(u.getPasswordHash());
    q.addBindValue(u.getBirthDate().toString(Qt::ISODate));
    q.addBindValue(u.getHeightCm());
    q.addBindValue(u.getWeightKg());

    return q.exec();
}

std::optional<UserModel> UserRepository::getByLogin(const QString& login) {
    QSqlQuery q;
    q.prepare("SELECT * FROM users WHERE login = ?");
    q.addBindValue(login);

    if (!q.exec() || !q.next())
        return std::nullopt;

    UserModel u;
    u.setId(q.value("id").toInt());
    u.setLogin(q.value("login").toString());
    u.setPasswordHash(q.value("password_hash").toString());
    u.setBirthDate(QDate::fromString(q.value("birth_date").toString(), Qt::ISODate));
    u.setHeightCm(q.value("height_cm").toInt());
    u.setWeightKg(q.value("weight_kg").toDouble());

    return u;
}

UserModel* UserRepository::loginValid(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT id, login, password_hash, birth_date, height_cm, weight_kg FROM users WHERE login = :login");
    query.bindValue(":login", username);

    if (!query.exec() || !query.next()) {
        return nullptr;  // Jeśli użytkownik nie istnieje lub wystąpił błąd
    }

    QString storedPassword = query.value(2).toString();

    if (storedPassword == password) {  // Sprawdzanie hasła
        UserModel* user = new UserModel();  // Tworzymy obiekt dynamicznie
        user->setId(query.value(0).toInt());  // Poprawiona linia z setId
        user->setLogin(query.value(1).toString());
        user->setPasswordHash(query.value(2).toString());
        user->setBirthDate(query.value(3).toDate());
        user->setHeightCm(query.value(4).toInt());
        user->setWeightKg(query.value(5).toDouble());


        return user;  // Zwracamy wskaźnik do obiektu
    }

    return nullptr;  // Złe dane logowania
}

