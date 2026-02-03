#include <QApplication>
#include "./Database/databasemanager.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 🔥 TO JEST KLUCZOWE
    if (!DatabaseManager::instance().connect()) {
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
