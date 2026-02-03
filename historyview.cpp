#include "historyview.h"
#include "ui_historyview.h"
#include "./Database/databasemanager.h"
#include <QSqlQuery>

HistoryView::HistoryView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryView)
{
    ui->setupUi(this);

    // Rozciąganie kolumn na szerokość okna
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->btnRefresh, &QPushButton::clicked, this, &HistoryView::loadHistory);
}

HistoryView::~HistoryView()
{
    delete ui;
}

void HistoryView::setUserData(UserModel *currentUser)
{
    this->currentUser = currentUser;
    loadHistory();
}

void HistoryView::loadHistory()
{
    if (!currentUser) return;

    ui->tableHistory->setRowCount(0);

    QSqlQuery q(DatabaseManager::instance().database());

    // 🔥 ZMIANA: Dodałem ORDER BY s.date DESC, s.id DESC
    // To sortuje datami od najnowszej.
    // "s.id DESC" to zabezpieczenie: jeśli masz dwa treningi tego samego dnia,
    // nowszy (z wyższym ID) będzie wyżej.
    q.prepare("SELECT s.id, s.date, w.name "
              "FROM workout_sessions s "
              "JOIN workouts w ON s.workout_id = w.id "
              "WHERE s.user_id = :uid "
              "ORDER BY s.date DESC, s.id DESC");

    q.bindValue(":uid", currentUser->getId());

    if (q.exec()) {
        while (q.next()) {
            int row = ui->tableHistory->rowCount();
            ui->tableHistory->insertRow(row);

            ui->tableHistory->setItem(row, 0, new QTableWidgetItem(q.value(1).toString())); // Data
            ui->tableHistory->setItem(row, 1, new QTableWidgetItem(q.value(2).toString())); // Nazwa
            ui->tableHistory->setItem(row, 2, new QTableWidgetItem(q.value(0).toString())); // ID
        }
    }
}
