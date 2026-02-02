#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    QSqlDatabase database();
    bool connect();

private:
    DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void initDatabase();
};

#endif // DATABASEMANAGER_H
