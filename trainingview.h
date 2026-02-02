#ifndef TRAININGVIEW_H
#define TRAININGVIEW_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
#include <QLabel>
#include "./Model/usermodel.h"

namespace Ui {
class trainingview;
}

// Struktura do łatwego odczytu danych z formularza
struct ExerciseInputGroup {
    int exerciseId;
    QLineEdit* weightInput;
    QLineEdit* repsInput;
};

class trainingview : public QWidget
{
    Q_OBJECT

public:
    explicit trainingview(QWidget *parent = nullptr);
    ~trainingview();

    void setUserData(UserModel* user);
    void refreshWorkoutList();

private slots:
    void on_listWorkouts_itemClicked(); // Wybór treningu z listy
    void on_btnSaveWorkout_clicked();   // Zapis do bazy

private:
    Ui::trainingview *ui;
    UserModel* currentUser = nullptr;
    QList<ExerciseInputGroup> activeInputs;
    int selectedWorkoutId = -1;

    void clearForm();
};

#endif // TRAININGVIEW_H
