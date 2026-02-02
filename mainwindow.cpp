#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LoginView.h"  // Dodanie LoginView
#include "DashboardView.h"
#include "WorkoutsView.h"
#include "HistoryView.h"
#include "ProfileView.h"
#include "trainingview.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dashboardView(nullptr)
    , workoutsView(nullptr)
    , historyView(nullptr)
    , profileView(nullptr)
    , loginView(nullptr)
    , trainingView(nullptr)// Dodanie loginView
{
    ui->setupUi(this);

    setupViews();
    connectUi();

    // Na początku ustawienie przycisków jako zablokowane
    lockButtons();

    // Na początku pokazujemy okno logowania
    ui->stackedWidget->setCurrentIndex(0); // Ustawienie LoginView jako pierwszy widok

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupViews()
{
    // Usuwamy placeholderowe strony z .ui
    while (ui->stackedWidget->count() > 0) {
        QWidget* w = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(w);
        w->deleteLater();
    }

    // Dodajemy loginView, dashboardView itd.
    loginView = new LoginView(this);
    dashboardView = new DashboardView(this);
    workoutsView  = new WorkoutsView(this);
    historyView   = new HistoryView(this);
    profileView   = new ProfileView(this);
    trainingView = new trainingview(this);
    ui->stackedWidget->addWidget(loginView);  // Okno logowania jest pierwszym widgetem
    ui->stackedWidget->addWidget(dashboardView);
    ui->stackedWidget->addWidget(workoutsView);
    ui->stackedWidget->addWidget(historyView);
    ui->stackedWidget->addWidget(profileView);
    ui->stackedWidget->addWidget(trainingView);
}

void MainWindow::connectUi()
{   connect(loginView, &LoginView::loginSuccessful, this, &MainWindow::onLoginSuccessful);

    connect(ui->btnDashboard, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(dashboardView);
    });

    connect(ui->btnWorkouts, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(workoutsView);
    });

    connect(ui->btnHistory, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(historyView);
    });

    connect(ui->btnProfile, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(profileView);
    });

    connect(ui->btnExit, &QPushButton::clicked, this, []() {
        QApplication::quit();
    });
    connect(dashboardView, &DashboardView::requestWorkoutTab, this, [this]() {
        ui->stackedWidget->setCurrentWidget(trainingView);
        // Opcjonalnie: trainingView->refreshData();
    });
    connect(ui->btnTraining, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(trainingView);
        trainingView->refreshWorkoutList(); // Wymuszamy pobranie nowych planów z bazy
    });

    // To samo dla przycisku z Dashboardu:
    connect(dashboardView, &DashboardView::requestWorkoutTab, this, [this]() {
        ui->stackedWidget->setCurrentWidget(trainingView);
        trainingView->refreshWorkoutList(); // Odśwież listę przed pokazaniem okna
    });
}

void MainWindow::lockButtons()
{
    // Zablokowanie przycisków do momentu zalogowania
    ui->btnDashboard->setEnabled(false);
    ui->btnWorkouts->setEnabled(false);
    ui->btnHistory->setEnabled(false);
    ui->btnProfile->setEnabled(false);
    ui->btnTraining->setEnabled(false);
}

void MainWindow::unlockButtons()
{
    // Odblokowanie przycisków po zalogowaniu
    ui->btnDashboard->setEnabled(true);
    ui->btnWorkouts->setEnabled(true);
    ui->btnHistory->setEnabled(true);
    ui->btnProfile->setEnabled(true);
    ui->btnTraining->setEnabled(true);
}

void MainWindow::onLoginSuccessful(UserModel* user)
{
    currentUser = user;  // Store the logged-in user object

    // Display user info (optional)
    qDebug() << "Logged in as: " << currentUser->getLogin();

    // Enable buttons and show dashboard
    unlockButtons();
    ui->stackedWidget->setCurrentWidget(dashboardView);

    dashboardView->setUserData(currentUser);
    profileView->setUserData(currentUser);
    historyView->setUserData(currentUser);
    workoutsView->setUserData(currentUser);
    trainingView->setUserData(currentUser);
}
