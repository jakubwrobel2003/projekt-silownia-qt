#include <QApplication>
#include "MainWindow.h"
#include "./Database/databasemanager.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🔥 TO JEST KLUCZOWE
    if (!DatabaseManager::instance().connect())
    {
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}

