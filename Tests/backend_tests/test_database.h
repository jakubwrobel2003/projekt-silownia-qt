#ifndef TEST_DATABASE_H
#define TEST_DATABASE_H

#include <QObject>
#include <QtTest>
#include "Database/databasemanager.h"

class TestDatabase : public QObject {
    Q_OBJECT
private slots:
    void connectToDatabase() {
        QVERIFY(DatabaseManager::connect());
    }
};

#endif // TEST_DATABASE_H
