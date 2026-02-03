#include "dashboardview.h"
#include "ui_dashboardview.h"
#include "./Database/databasemanager.h"
#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextCharFormat>
#include <QVBoxLayout>

DashboardView::DashboardView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardView)
{
    ui->setupUi(this);

    // Domyślne daty
    ui->dateFrom->setDate(QDate::currentDate().addMonths(-3)); // Ostatnie 3 miesiące domyślnie
    ui->dateTo->setDate(QDate::currentDate());

    loadExerciseList();
    setupChart(); // Konfiguracja wykresu

    // Połączenia
    connect(ui->btnRefreshAnalysis, &QPushButton::clicked, this, &DashboardView::updateAnalysis);

    // Konfiguracja tabeli
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

DashboardView::~DashboardView()
{
    delete ui;
}

// =========================================================
// KONFIGURACJA WYKRESU (DARK MODE)
// =========================================================
void DashboardView::setupChart()
{
    chart = new QChart();
    series = new QLineSeries();

    // Styl serii danych
    QPen pen(QColor("#00E5FF")); // Neonowy cyjan
    pen.setWidth(3);
    series->setPen(pen);

    chart->addSeries(series);
    chart->setTitle("Postęp Siłowy (Ciężar)");

    // Ustawiamy ciemny motyw wykresu
    chart->setTheme(QChart::ChartThemeDark);
    chart->setBackgroundBrush(QBrush(QColor("#232336"))); // Tło pasujące do UI
    chart->setTitleBrush(QBrush(Qt::white));
    chart->legend()->setLabelColor(Qt::white);

    // OSIE
    axisX = new QDateTimeAxis;
    axisX->setFormat("dd.MM");
    axisX->setTitleText("Data");
    axisX->setLabelsColor(Qt::lightGray);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setTitleText("Ciężar (kg)");
    axisY->setLabelsColor(Qt::lightGray);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // WIDOK
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Dodajemy do kontenera w UI
    if (ui->chartContainer) {
        QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(chartView);
    }
}

void DashboardView::setUserData(UserModel *currentUser)
{
    this->currentUser = currentUser;
    if (currentUser) {
        ui->lblWelcome->setText(QString("Witaj, %1!").arg(currentUser->getLogin()));
        ui->lblCurrentDate->setText(QDateTime::currentDateTime().toString("dd MMMM yyyy"));

        updateCalendarMarks();
        updateMonthlyStats(); // Licznik treningów
    }
}

void DashboardView::updateCalendarMarks()
{
    if (!currentUser) return;

    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT DISTINCT date FROM workout_sessions WHERE user_id = :uid");
    q.bindValue(":uid", currentUser->getId());

    QTextCharFormat eventFormat;
    eventFormat.setBackground(QColor("#4CAF50")); // Zielony
    eventFormat.setForeground(Qt::white);
    eventFormat.setFontWeight(QFont::Bold);

    if (q.exec()) {
        while (q.next()) {
            QString dateStr = q.value(0).toString();
            QDate workoutDate = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (workoutDate.isValid()) {
                ui->calendarWidget->setDateTextFormat(workoutDate, eventFormat);
            }
        }
    }
}

void DashboardView::updateMonthlyStats()
{
    if (!currentUser) return;

    QDate now = QDate::currentDate();
    QString start = QDate(now.year(), now.month(), 1).toString("yyyy-MM-dd");

    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT COUNT(*) FROM workout_sessions WHERE user_id = :uid AND date >= :start");
    q.bindValue(":uid", currentUser->getId());
    q.bindValue(":start", start);

    if(q.exec() && q.next()) {
        ui->lblMonthCount->setText(q.value(0).toString());
    }
}

void DashboardView::loadExerciseList()
{
    ui->comboExercises->clear();
    QSqlQuery q("SELECT name FROM exercise_definitions ORDER BY name",
                DatabaseManager::instance().database());
    while (q.next()) {
        ui->comboExercises->addItem(q.value(0).toString());
    }
}

void DashboardView::updateAnalysis()
{
    if (!currentUser) return;

    series->clear();
    ui->tableHistory->setRowCount(0);

    QString exerciseName = ui->comboExercises->currentText();
    QString start = ui->dateFrom->date().toString("yyyy-MM-dd");
    QString end = ui->dateTo->date().toString("yyyy-MM-dd");

    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT ws.date, sr.weight, sr.reps "
              "FROM strength_results sr "
              "JOIN exercise_results er ON sr.result_id = er.id "
              "JOIN workout_sessions ws ON er.session_id = ws.id "
              "JOIN exercise_definitions ed ON er.exercise_id = ed.id "
              "WHERE ed.name = :name AND ws.user_id = :uid "
              "AND ws.date BETWEEN :start AND :end "
              "ORDER BY ws.date ASC");

    q.bindValue(":name", exerciseName);
    q.bindValue(":uid", currentUser->getId());
    q.bindValue(":start", start);
    q.bindValue(":end", end);

    // 🔥 1. ZMIENNA NA REKORD ŻYCIOWY
    double globalMax1RM = 0.0;

    if (q.exec()) {
        QDateTime minDate, maxDate;
        double maxWeight = 0;
        double minWeight = 1000;
        bool hasData = false;

        while (q.next()) {
            hasData = true;
            QString dateStr = q.value(0).toString();
            double weight = q.value(1).toDouble();
            int reps = q.value(2).toInt();

            // 🔥 2. OBLICZANIE 1RM (Wzór Epleya) DLA KAŻDEGO TRENINGU
            double current1RM = weight;
            if (reps > 1) {
                current1RM = weight * (1.0 + (double)reps / 30.0);
            }

            // Jeśli ten trening był lepszy niż dotychczasowy rekord, nadpisz go
            if (current1RM > globalMax1RM) {
                globalMax1RM = current1RM;
            }

            QDate dateVal = QDate::fromString(dateStr, "yyyy-MM-dd");
            QDateTime dt(dateVal, QTime(0, 0));

            // Dodaj punkt do wykresu
            series->append(dt.toMSecsSinceEpoch(), weight);

            // Dodaj do tabeli
            int row = ui->tableHistory->rowCount();
            ui->tableHistory->insertRow(row);
            ui->tableHistory->setItem(row, 0, new QTableWidgetItem(dateVal.toString("dd.MM.yyyy")));
            ui->tableHistory->setItem(row, 1, new QTableWidgetItem(QString::number(weight) + " kg"));
            ui->tableHistory->setItem(row, 2, new QTableWidgetItem(QString::number(reps)));

            // Ekstrema
            if (!minDate.isValid() || dt < minDate) minDate = dt;
            if (!maxDate.isValid() || dt > maxDate) maxDate = dt;
            if (weight > maxWeight) maxWeight = weight;
            if (weight < minWeight) minWeight = weight;
        }

        if (hasData) {
            series->setPointsVisible(true);

            // Ustawienie osi
            axisX->setRange(minDate.addDays(-2), maxDate.addDays(2));
            axisY->setRange(minWeight - 5, maxWeight + 5);

            // 🔥 3. WYŚWIETLENIE WYNIKU NA ETYKIECIE
            ui->lblMax1RM->setText(QString("REKORD (1RM): %1 kg").arg(QString::number(globalMax1RM, 'f', 1)));

            chart->update();
            chartView->repaint();
        } else {
            // Jeśli nie ma danych, wyczyść etykietę
            ui->lblMax1RM->setText("Brak danych");
        }
    }
}

void DashboardView::on_btnRefreshAnalysis_clicked()
{
    updateAnalysis();
}
