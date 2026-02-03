#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDebug>

// Includujemy nagłówki wszystkich widoków
#include "LoginView.h"
#include "DashboardView.h"
#include "WorkoutsView.h"
#include "HistoryView.h"
#include "ProfileView.h"
#include "trainingview.h"
#include "helpview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginView(nullptr)
    , dashboardView(nullptr)
    , workoutsView(nullptr)
    , historyView(nullptr)
    , profileView(nullptr)
    , trainingView(nullptr)
    , helpView(nullptr)
{
    ui->setupUi(this);

    // 1. Inicjalizacja widoków
    setupViews();

    // 2. Podłączenie sygnałów i slotów
    connectUi();

    // 3. Stan początkowy (zablokowane menu, widok logowania)
    lockButtons();
    ui->stackedWidget->setCurrentWidget(loginView);

    // =================================================================
    // 🎨 STYLIZACJA (DARK NAVY)
    // =================================================================
    this->setStyleSheet(
        "QMainWindow, QWidget, QStackedWidget, QFrame { "
        "   background-color: #1B1B2F; "
        "   color: #E0E0E0; "
        "   font-family: 'Segoe UI', sans-serif;"
        "   font-size: 14px;"
        "} "

        // RAMKA GÓRNA
        "QFrame#frame { "
        "   background-color: #1B1B2F; "
        "   border: none; "
        "} "

        // PRZYCISKI MENU
        "QPushButton { "
        "   background-color: #2E2E40; "
        "   color: white; "
        "   border: 1px solid #4B4B6E; "
        "   border-radius: 5px; "
        "   padding: 8px 15px; "
        "   font-weight: bold;"
        "} "
        "QPushButton:hover { background-color: #3E3E5E; border-color: #6C6C93; } "
        "QPushButton:pressed { background-color: #1F1F2E; } "

        // INPUTY I FORMULARZE
        "QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox, QDateEdit { "
        "   background-color: #232336; "
        "   border: 1px solid #4B4B6E; "
        "   border-radius: 4px; "
        "   padding: 5px; "
        "   color: white; "
        "} "

        // GRUPY (QGroupBox)
        "QGroupBox { "
        "   border: 1px solid #4B4B6E; "
        "   border-radius: 6px; "
        "   margin-top: 24px; "
        "   font-weight: bold; "
        "   padding-top: 10px; "
        "} "
        "QGroupBox::title { "
        "   subcontrol-origin: margin; "
        "   left: 10px; "
        "   padding: 0 5px; "
        "   color: #4CAF50; "
        "} "

        // TABELE
        "QTableWidget, QListWidget { "
        "   background-color: #232336; "
        "   gridline-color: #3E3E5E; "
        "   border: 1px solid #4B4B6E; "
        "   color: white; "
        "} "
        "QHeaderView::section { "
        "   background-color: #2E2E40; "
        "   color: #E0E0E0; "
        "   padding: 4px; "
        "   border: 1px solid #3E3E5E; "
        "} "

        // ZIELONE PRZYCISKI (Akcje główne)
        "QPushButton#loginButton, QPushButton#btnSaveWorkout, QPushButton#btnAddMeasurement, QPushButton#btnStartWorkout { "
        "   background-color: #4CAF50; "
        "   border: none; "
        "} "
        "QPushButton#loginButton:hover, QPushButton#btnSaveWorkout:hover, QPushButton#btnAddMeasurement:hover, QPushButton#btnStartWorkout:hover { "
        "   background-color: #45a049; "
        "} "

        // FIOLETOWE/CYJANOWE PRZYCISKI (Akcje dodatkowe/AI)
        "QPushButton#generateAiWorkoutButton, QPushButton#btnCalculateCalories, QPushButton#btnChangePhoto, QPushButton#btnRefreshAnalysis, QPushButton#btnCalculate1RM { "
        "   background-color: #5C6BC0; "
        "   border: none; "
        "} "
        "QPushButton#generateAiWorkoutButton:hover, QPushButton#btnCalculateCalories:hover, QPushButton#btnChangePhoto:hover, QPushButton#btnRefreshAnalysis:hover, QPushButton#btnCalculate1RM:hover { "
        "   background-color: #3F51B5; "
        "} "
        );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupViews()
{
    // Czyścimy stos widgetów (na wszelki wypadek)
    while (ui->stackedWidget->count() > 0) {
        QWidget *w = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(w);
        w->deleteLater();
    }

    // Tworzymy instancje wszystkich widoków
    loginView = new LoginView(this);
    dashboardView = new DashboardView(this);
    workoutsView = new WorkoutsView(this);
    historyView = new HistoryView(this);
    profileView = new ProfileView(this);
    trainingView = new trainingview(this);
    helpView = new HelpView(this); // Nowy widok pomocy

    // Dodajemy je do StackedWidget w odpowiedniej kolejności
    ui->stackedWidget->addWidget(loginView);      // Index 0
    ui->stackedWidget->addWidget(dashboardView);  // Index 1
    ui->stackedWidget->addWidget(workoutsView);   // Index 2
    ui->stackedWidget->addWidget(historyView);    // Index 3
    ui->stackedWidget->addWidget(profileView);    // Index 4
    ui->stackedWidget->addWidget(trainingView);   // Index 5
    ui->stackedWidget->addWidget(helpView);       // Index 6
}

void MainWindow::connectUi()
{
    // Logowanie
    connect(loginView, &LoginView::loginSuccessful, this, &MainWindow::onLoginSuccessful);

    // Nawigacja - Menu Główne
    connect(ui->btnDashboard, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(dashboardView);
        // Opcjonalnie: dashboardView->updateAnalysis(); // jeśli chcesz odświeżać przy każdym wejściu
    });

    connect(ui->btnWorkouts, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(workoutsView);
    });

    connect(ui->btnHistory, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(historyView);
        historyView->loadHistory(); // Odśwież historię przy wejściu
    });

    connect(ui->btnProfile, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(profileView);
    });

    // Przycisk "Trening" - wymaga odświeżenia listy planów
    connect(ui->btnTraining, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(trainingView);
        trainingView->refreshWorkoutList();
    });

    // Przycisk "Pomoc"
    connect(ui->btnHelp, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(helpView);
    });

    // Przycisk "Wyjście"
    connect(ui->btnExit, &QPushButton::clicked, this, []() {
        QApplication::quit();
    });
}

void MainWindow::lockButtons()
{
    ui->btnDashboard->setEnabled(false);
    ui->btnWorkouts->setEnabled(false);
    ui->btnHistory->setEnabled(false);
    ui->btnProfile->setEnabled(false);
    ui->btnTraining->setEnabled(false);
    ui->btnHelp->setEnabled(false); // Blokujemy też pomoc przed zalogowaniem (opcjonalnie)
}

void MainWindow::unlockButtons()
{
    ui->btnDashboard->setEnabled(true);
    ui->btnWorkouts->setEnabled(true);
    ui->btnHistory->setEnabled(true);
    ui->btnProfile->setEnabled(true);
    ui->btnTraining->setEnabled(true);
    ui->btnHelp->setEnabled(true);
}

void MainWindow::onLoginSuccessful(UserModel *user)
{
    currentUser = user;
    qDebug() << "Logged in as: " << currentUser->getLogin();

    unlockButtons();

    // Przekazujemy dane użytkownika do wszystkich widoków
    dashboardView->setUserData(currentUser);
    profileView->setUserData(currentUser);
    historyView->setUserData(currentUser);
    workoutsView->setUserData(currentUser);
    trainingView->setUserData(currentUser);
    // HelpView nie potrzebuje usera, bo wyświetla tylko tekst

    // Przełączamy na Dashboard
    ui->stackedWidget->setCurrentWidget(dashboardView);
}
