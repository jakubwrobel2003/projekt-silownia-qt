#ifndef DASHBOARDVIEW_H
#define DASHBOARDVIEW_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include "./Model/usermodel.h"

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

private slots:
    void updateAnalysis(); // Slot odświeżania wykresu
    void on_btnRefreshAnalysis_clicked();

private:
    Ui::DashboardView *ui;
    UserModel *currentUser = nullptr;

    void loadExerciseList();
    void updateCalendarMarks(); // Kolorowanie dni treningowych
    void updateMonthlyStats();  // Nowa funkcja do licznika

    // Komponenty wykresu
    QChartView *chartView;
    QChart *chart;
    QLineSeries *series;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;

    void setupChart(); // Konfiguracja wyglądu wykresu
};

#endif // DASHBOARDVIEW_H
