#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DashboardView.h"
#include "WorkoutsView.h"
#include "HistoryView.h"
#include "ProfileView.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dashboardView(nullptr)
    , workoutsView(nullptr)
    , historyView(nullptr)
    , profileView(nullptr)
{
    ui->setupUi(this);

    setupViews();
    connectUi();

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupViews()
{
    // usuwamy placeholderowe strony z .ui
    while (ui->stackedWidget->count() > 0) {
        QWidget* w = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(w);
        w->deleteLater();
    }

    dashboardView = new DashboardView(this);
    workoutsView  = new WorkoutsView(this);
    historyView   = new HistoryView(this);
    profileView   = new ProfileView(this);

    ui->stackedWidget->addWidget(dashboardView);
    ui->stackedWidget->addWidget(workoutsView);
    ui->stackedWidget->addWidget(historyView);
    ui->stackedWidget->addWidget(profileView);
}


void MainWindow::connectUi()
{
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
}
