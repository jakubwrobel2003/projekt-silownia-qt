#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_startWorkoutButton_clicked() {
    trainingManager.startWorkout(currentUserId, currentWorkoutId);
}

void MainWindow::on_addStrengthButton_clicked() {
    trainingManager.saveStrengthExercise(
        1,      // exerciseId
        80.0,   // weight
        8,      // reps
        7       // difficulty
        );
}

void MainWindow::on_addCardioButton_clicked() {
    trainingManager.saveCardioExercise(
        2,     // exerciseId
        2.5,   // distance
        6      // difficulty
        );
}

void MainWindow::on_finishWorkoutButton_clicked() {
    trainingManager.finishWorkout();
}
