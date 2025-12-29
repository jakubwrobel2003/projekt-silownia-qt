#include <QCoreApplication>
#include <QtTest>

// Dołącz swoje klasy testowe (pliki .h albo .cpp z definicją klas testów)
#include "test_database.h"
#include "test_userrepository.h"
#include "test_workoutflow.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int status = 0;

    // Uruchamiamy testy po kolei
    {
        TestDatabase tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TestUserRepository tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        TestWorkoutFlow tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    return status;
}
