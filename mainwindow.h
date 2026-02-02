#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "./Model/usermodel.h"
#include <QMainWindow>

// forward declarations
class DashboardView;
class WorkoutsView;
class HistoryView;
class ProfileView;
class LoginView;
class trainingview;
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
    LoginView*     loginView;
    trainingview* trainingView;
    bool isLog=false;
    void setupViews();
    void connectUi();
    void onLoginSuccessful(UserModel* user);
    void lockButtons();
    void unlockButtons();
    UserModel* currentUser=nullptr;
};

#endif // MAINWINDOW_H
