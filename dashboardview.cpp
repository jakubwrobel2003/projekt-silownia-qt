#include "dashboardview.h"
#include "ui_dashboardview.h"
#include "./Database/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QTextCharFormat>

DashboardView::DashboardView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardView)
{
    ui->setupUi(this);

    loadExerciseList();

    // Ustawienie domyślnego zakresu dat
    ui->dateFrom->setDate(QDate::currentDate().addMonths(-1));
    ui->dateTo->setDate(QDate::currentDate());

    // Połączenia sygnałów
    connect(ui->btnRefreshAnalysis, &QPushButton::clicked, this, &DashboardView::updateAnalysis);
    connect(ui->btnStartWorkout, &QPushButton::clicked, this, [this](){
        emit requestWorkoutTab();
    });

    // W konstruktorze DashboardView:
    chart = new QChart();
    series = new QLineSeries();
    chart->addSeries(series);
    chart->setTitle("Postęp Siłowy");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Podpięcie pod Twój kontener z Designera
    if (ui->chartContainer) {
        QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
        layout->addWidget(chartView);
    }

    // Konfiguracja osi
    axisX = new QDateTimeAxis;
    axisX->setFormat("dd.MM");
    axisX->setTitleText("Data");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setTitleText("Ciężar (kg)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}

DashboardView::~DashboardView() {
    delete ui;
}

void DashboardView::setUserData(UserModel* currentUser) {
    this->currentUser = currentUser;
    if (currentUser) {
        ui->lblWelcome->setText(QString("Witaj, %1!").arg(currentUser->getLogin()));
        ui->lblCurrentDate->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm"));
        updateCalendarMarks();
        updateLastWorkoutLink();
    }
}

void DashboardView::updateCalendarMarks() {
    if (!currentUser) return;

    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT DISTINCT date FROM workout_sessions WHERE user_id = :uid");
    q.bindValue(":uid", currentUser->getId());

    QTextCharFormat eventFormat;
    eventFormat.setBackground(Qt::green);
    eventFormat.setForeground(Qt::white);
    eventFormat.setFontWeight(QFont::Bold);

    if (q.exec()) {
        while (q.next()) { // q.next() przesuwa kursor na rekord
            // Pobieramy wartość z pierwszej kolumny (indeks 0) i zamieniamy na string
            QString dateStr = q.value(0).toString();
            QDate workoutDate = QDate::fromString(dateStr, "yyyy-MM-dd");

            if (workoutDate.isValid()) {
                ui->calendarWidget->setDateTextFormat(workoutDate, eventFormat);
            }
        }
    }
}

void DashboardView::loadExerciseList() {
    ui->comboExercises->clear();
    QSqlQuery q("SELECT name FROM exercise_definitions ORDER BY name", DatabaseManager::instance().database());
    while (q.next()) {
        ui->comboExercises->addItem(q.value(0).toString());
    }
}

void DashboardView::updateAnalysis() {
    if (!currentUser) return;

    // 1. Reset danych i interfejsu
    series->clear();
    ui->tableHistory->setRowCount(0);

    QString exerciseName = ui->comboExercises->currentText();
    QString start = ui->dateFrom->date().toString("yyyy-MM-dd");
    QString end = ui->dateTo->date().toString("yyyy-MM-dd");

    // 2. Pobranie danych z bazy
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

    if (q.exec()) {
        QDateTime minDate, maxDate;
        double maxWeight = 0;
        bool hasData = false;

        while (q.next()) {
            hasData = true;
            QString dateStr = q.value(0).toString();
            double weight = q.value(1).toDouble();
            int reps = q.value(2).toInt();

            QDate dateVal = QDate::fromString(dateStr, "yyyy-MM-dd");
            QDateTime dt(dateVal, QTime(0, 0));

            // Logowanie do konsoli dla Twojej kontroli
            qDebug() << "Punkt wykresu -> Data:" << dateStr << "Ciężar:" << weight;

            // Dodanie punktu do serii (X musi być w milisekundach)
            series->append(dt.toMSecsSinceEpoch(), weight);

            // Aktualizacja tabeli
            int row = ui->tableHistory->rowCount();
            ui->tableHistory->insertRow(row);
            ui->tableHistory->setItem(row, 0, new QTableWidgetItem(dateVal.toString("dd.MM.yyyy")));
            ui->tableHistory->setItem(row, 1, new QTableWidgetItem(QString::number(weight) + " kg"));
            ui->tableHistory->setItem(row, 2, new QTableWidgetItem(QString::number(reps)));

            // Obliczanie ekstremów dla osi
            if (!minDate.isValid() || dt < minDate) minDate = dt;
            if (!maxDate.isValid() || dt > maxDate) maxDate = dt;
            if (weight > maxWeight) maxWeight = weight;
        }

        // 3. Dynamiczne ustawianie osi (Naprawa "pustego" wykresu)
        if (hasData) {
            // Ustawiamy kropki na linii, żeby były widoczne
            series->setPointsVisible(true);
            series->setPointLabelsVisible(true);

            // Ustawienie zakresów (dodajemy margines, by punkty nie "kleiły" się do brzegów)
            axisX->setRange(minDate.addDays(-1), maxDate.addDays(1));
            axisY->setRange(0, maxWeight + 10);

            // Wymuszenie odświeżenia całego komponentu
            chart->update();
            chartView->repaint();
        } else {
            qDebug() << "Baza zwróciła 0 wyników dla:" << exerciseName << "w zakresie" << start << "-" << end;
        }
    } else {
        qDebug() << "Błąd SQL w updateAnalysis:" << q.lastError().text();
    }
}
void DashboardView::updateLastWorkoutLink() {
    qDebug() << "Przycisk Odśwież został kliknięty!";
    if (!currentUser) return;
    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT ws.date, w.name FROM workout_sessions ws "
              "JOIN workouts w ON ws.workout_id = w.id "
              "WHERE ws.user_id = :uid ORDER BY ws.date DESC LIMIT 1");
    q.bindValue(":uid", currentUser->getId());

    if (q.exec() && q.next()) {
        ui->btnLastWorkout->setText(QString("Ostatni trening: %1 (%2)")
                                        .arg(q.value(1).toString())
                                        .arg(q.value(0).toString()));
    }
}

void DashboardView::on_btnRefreshAnalysis_clicked()
{
    updateAnalysis();
    updateLastWorkoutLink();

}

