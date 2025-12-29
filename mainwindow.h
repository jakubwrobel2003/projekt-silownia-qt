#include <QMainWindow>
#include "Model/TrainingManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startWorkoutButton_clicked();
    void on_addStrengthButton_clicked();
    void on_addCardioButton_clicked();
    void on_finishWorkoutButton_clicked();

private:
    Ui::MainWindow *ui;
    TrainingManager trainingManager;

    int currentUserId = 1;    // tymczasowo
    int currentWorkoutId = 1; // tymczasowo
};
