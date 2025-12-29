#include "databasemanager.h"
#include <QSqlError>
#include <QDebug>

bool DatabaseManager::connect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("silownia.db");

    if (!db.open()) {
        qDebug() << "❌ DB ERROR:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ DB CONNECTED";
    return true;
}
