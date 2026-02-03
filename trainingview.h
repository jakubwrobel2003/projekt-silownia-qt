#ifndef TRAININGVIEW_H
#define TRAININGVIEW_H

#include <QWidget>
#include <QListWidgetItem>
#include "./Model/usermodel.h"
#include "./Model/trainingmanager.h"

namespace Ui {
class trainingview;
}

class trainingview : public QWidget
{
    Q_OBJECT

public:
    explicit trainingview(QWidget *parent = nullptr);
    ~trainingview();

    void setUserData(UserModel *user);
    void refreshWorkoutList();

private slots:
    void on_listWorkouts_itemClicked();
    void on_btnSaveWorkout_clicked(); // Slot zapisu

private:
    Ui::trainingview *ui;
    UserModel *currentUser = nullptr;
    TrainingManager *trainingManager;

    int selectedWorkoutId = -1;
};

#endif // TRAININGVIEW_H
