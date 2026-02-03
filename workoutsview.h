#ifndef WORKOUTSVIEW_H
#define WORKOUTSVIEW_H

#include <QWidget>
#include "./AI/aiworkoutgenerator.h"
#include "./AI/aiworkoutplan.h"
#include "./Model/cardioexercise.h"
#include "./Model/exercise.h"
#include "./Model/strengthexercise.h"
#include "./Model/trainingmanager.h"
#include "./Model/usermodel.h"
#include "./Model/workout.h"
#include <memory>
namespace Ui {
class WorkoutsView;
}

class WorkoutsView : public QWidget
{
    Q_OBJECT

public:
    explicit WorkoutsView(QWidget *parent = nullptr);
    ~WorkoutsView();

    void setUserData(UserModel *currentUser);

private slots:
    void onGenerateAiWorkoutClicked();
    void onAiPlanReady(const AIWorkoutPlan &plan);
    void onAddExerciseClicked();
    void onSaveWorkoutClicked();
    void onRemoveExerciseClicked();

private:
    void loadExercises(const QString &type);
    TrainingManager *trainingManager;
    QList<WorkoutExercisePlan> currentPlan;
    Ui::WorkoutsView *ui;

    UserModel *currentUser = nullptr;
    Workout currentWorkout;

    int editedExerciseIndex = -1;
};

#endif // WORKOUTSVIEW_H
