#ifndef WORKOUTSVIEW_H
#define WORKOUTSVIEW_H

#include <QWidget>
#include <QMap> // <--- WAŻNE: Dodane do obsługi linków
#include "./AI/aiworkoutgenerator.h"
#include "./AI/aiworkoutplan.h"
#include "./Model/trainingmanager.h"
#include "./Model/usermodel.h"
#include "./Model/workout.h"

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

    // Nowy slot do ukrywania pól (Cardio vs Strength)
    void toggleInputFields();

    // Slot usuwania z tabeli (wywoływany przez przycisk w tabeli)
    void removeExerciseAt(int index);

    // 🔥 NOWY SLOT DO WIDEO
    void onShowVideoClicked();

private:
    void loadExercises(const QString &type);

    // Nowa funkcja do odświeżania tabeli
    void updateExerciseTable();

    // 🔥 Funkcja pomocnicza (żeby nie ruszać TrainingManagera)
    QString getExerciseNameById(int id);

    // 🔥 Inicjalizacja linków
    void initVideoLinks();

    TrainingManager *trainingManager;
    QList<WorkoutExercisePlan> currentPlan;
    Ui::WorkoutsView *ui;

    UserModel *currentUser = nullptr;

    // 🔥 Mapa linków
    QMap<QString, QString> exerciseLinks;
};

#endif // WORKOUTSVIEW_H
