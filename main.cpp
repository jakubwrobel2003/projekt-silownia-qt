#include <QApplication>
#include "mainwindow.h"
#include "Database/databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager::connect(); // test SQLite

    MainWindow w;
    w.show();

    return a.exec();
}
