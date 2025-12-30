#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// forward declarations
class DashboardView;
class WorkoutsView;
class HistoryView;
class ProfileView;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // widoki
    DashboardView* dashboardView;
    WorkoutsView*  workoutsView;
    HistoryView*   historyView;
    ProfileView*   profileView;

    void setupViews();
    void connectUi();
};

#endif // MAINWINDOW_H
