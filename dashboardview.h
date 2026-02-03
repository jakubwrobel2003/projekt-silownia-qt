#ifndef DASHBOARDVIEW_H
#define DASHBOARDVIEW_H

#include <QChartView>
#include <QDate>
#include <QLineSeries>
#include <QWidget>
#include <QtCharts>
#include "./Model/usermodel.h"
void setupChart();
namespace Ui {
class DashboardView;
}

class DashboardView : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardView(QWidget *parent = nullptr);
    ~DashboardView();
    void setUserData(UserModel *currentUser);

signals:
    void requestWorkoutTab(); // Sygnał do zmiany zakładki w MainWindow

private slots:
    void updateAnalysis(); // Slot dla przycisku odświeżania tabeli

    void on_btnRefreshAnalysis_clicked();

private:
    Ui::DashboardView *ui;
    UserModel *currentUser = nullptr;

    void loadExerciseList();
    void updateCalendarMarks();   // Kolorowanie dni w kalendarzu
    void updateLastWorkoutLink(); // Info o ostatnim treningu

    QChartView *chartView;
    QChart *chart;
    QLineSeries *series;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    void setupChart();
};

#endif
