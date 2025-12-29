#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "Model/UserModel.h"
#include <optional>

class UserRepository {
public:
    static bool createUser(const UserModel& user);
    static std::optional<UserModel> getByLogin(const QString& login);
    static bool loginValid(const QString& login, const QString& passwordHash);
};
#endif // USERREPOSITORY_H
