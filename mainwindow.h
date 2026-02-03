#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./Model/usermodel.h"

// Forward declarations (żeby nie zaśmiecać nagłówka includami)
namespace Ui { class MainWindow; }
class LoginView;
class DashboardView;
class WorkoutsView;
class HistoryView;
class ProfileView;
class trainingview; // Pamiętaj o małej literze, jeśli tak nazwałeś klasę
class HelpView;     // Nowy widok pomocy

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccessful(UserModel *user);

private:
    Ui::MainWindow *ui;

    // Wskaźniki na widoki
    LoginView *loginView;
    DashboardView *dashboardView;
    WorkoutsView *workoutsView;
    HistoryView *historyView;
    ProfileView *profileView;
    trainingview *trainingView;
    HelpView *helpView;

    UserModel *currentUser = nullptr;

    void setupViews();
    void connectUi();
    void lockButtons();
    void unlockButtons();
};

#endif // MAINWINDOW_H
