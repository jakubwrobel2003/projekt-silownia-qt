#include "profileview.h"
#include "ui_profileview.h"
#include "Database/databasemanager.h"
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>
#include <QDate>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

ProfileView::ProfileView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfileView)
{
    ui->setupUi(this);

    // Ustawienie proporcji
    ui->mainLayout->setStretch(0, 1);
    ui->mainLayout->setStretch(1, 2);
    ui->tableMeasurements->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setupCalorieCalculator();

    // Logo
    QPixmap log(":/new/logo/MacroMind logo with brain and barbell.png");
    if (!log.isNull()) {
        ui->logo_label->setPixmap(log.scaled(400, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->logo_label->setText("LOGO");
    }
    ui->logo_label->setAlignment(Qt::AlignCenter);
}

ProfileView::~ProfileView()
{
    delete ui;
}

// =========================================================
// KONFIGURACJA KALKULATORA
// =========================================================
void ProfileView::setupCalorieCalculator()
{
    ui->comboPAL->addItem("Siedzący tryb życia (1.2)", 1.2);
    ui->comboPAL->addItem("Lekka aktywność (1.375)", 1.375);
    ui->comboPAL->addItem("Umiarkowana (1.55)", 1.55);
    ui->comboPAL->addItem("Aktywny (1.725)", 1.725);
    ui->comboPAL->addItem("Bardzo aktywny (1.9)", 1.9);
    ui->comboPAL->setCurrentIndex(2);

    ui->comboGoal->addItem("Redukcja (-500 kcal)", -500);
    ui->comboGoal->addItem("Redukcja (-300 kcal)", -300);
    ui->comboGoal->addItem("Utrzymanie (0 kcal)", 0);
    ui->comboGoal->addItem("Masa (+300 kcal)", 300);
    ui->comboGoal->addItem("Masa (+500 kcal)", 500);
    ui->comboGoal->setCurrentIndex(2);
}

void ProfileView::setUserData(UserModel *currentUser)
{
    this->currentUser = currentUser;
    if (!currentUser) return;

    ui->LoginLabel->setText("Login: " + currentUser->getLogin());
    ui->birthDateLabel->setText("Data urodzin: " + currentUser->getBirthDate().toString("yyyy-MM-dd"));
    ui->spinWeight->setValue(currentUser->getWeightKg());
    ui->spinHeight->setValue(currentUser->getHeightCm());

    loadAvatar();
    loadMeasurementsHistory();
}

// =========================================================
// 🔥 OBLICZANIE KALORII I MAKRO
// =========================================================
void ProfileView::on_btnCalculateCalories_clicked()
{
    if (!currentUser) return;

    double weight = ui->spinWeight->value();
    double height = ui->spinHeight->value();

    QDate birth = currentUser->getBirthDate();
    QDate now = QDate::currentDate();
    int age = now.year() - birth.year();
    if (now.month() < birth.month() || (now.month() == birth.month() && now.day() < birth.day())) age--;

    if (weight <= 0 || height <= 0) {
        ui->lblCaloriesResult->setText("Uzupełnij wagę i wzrost!");
        return;
    }

    // 1. Oblicz BMR i TDEE
    double bmr = (10.0 * weight) + (6.25 * height) - (5.0 * age) + 5.0;
    double pal = ui->comboPAL->currentData().toDouble();
    int goalModifier = ui->comboGoal->currentData().toInt();

    double tdee = bmr * pal;
    double targetCalories = tdee + goalModifier;

    // 2. Dobór współczynników Makro (wg Twojego schematu)
    double proteinCoeff = 1.8; // Domyślnie dla utrzymania/masy (1.6 - 2.0)
    double fatCoeff = 1.0;     // Domyślnie dla utrzymania/masy (0.9 - 1.2)

    // Jeśli cel to redukcja (ujemne kalorie), zmieniamy współczynniki
    if (goalModifier < 0) {
        proteinCoeff = 2.2; // Redukcja (2.0 - 2.4) -> Środek to 2.2
        fatCoeff = 0.8;     // Redukcja (0.7 - 0.9) -> Środek to 0.8
    }

    // 3. Obliczenia Gramatury
    int proteinGrams = (int)(weight * proteinCoeff);
    int fatGrams = (int)(weight * fatCoeff);

    // Kalorie zajęte przez białko i tłuszcz
    int kcalFromProtein = proteinGrams * 4;
    int kcalFromFat = fatGrams * 9;

    // Reszta na węglowodany
    int kcalForCarbs = (int)targetCalories - kcalFromProtein - kcalFromFat;
    if (kcalForCarbs < 0) kcalForCarbs = 0; // Zabezpieczenie
    int carbGrams = kcalForCarbs / 4;

    // 4. Wyświetlanie wyniku (HTML Table)
    QString resultText = QString(
                             "<div style='text-align: center;'>"
                             "BMR: <b>%1</b> | TDEE: <b>%2</b><br>"
                             "<span style='font-size:16px; color:#4CAF50;'>CEL: <b>%3 kcal</b></span>"
                             "<hr style='border: 1px solid #4B4B6E;'>"
                             "<table width='100%' cellpadding='4'>"
                             "<tr>"
                             "<td style='color:#42A5F5;'><b>Białko:</b></td>"
                             "<td style='color:#FFCA28;'><b>Tłuszcze:</b></td>"
                             "<td style='color:#EF5350;'><b>Węgle:</b></td>"
                             "</tr>"
                             "<tr>"
                             "<td>%4 g</td>"
                             "<td>%5 g</td>"
                             "<td>%6 g</td>"
                             "</tr>"
                             "</table>"
                             "</div>"
                             ).arg((int)bmr)
                             .arg((int)tdee)
                             .arg((int)targetCalories)
                             .arg(proteinGrams)
                             .arg(fatGrams)
                             .arg(carbGrams);

    ui->lblCaloriesResult->setText(resultText);
}

// =========================================================
// POZOSTAŁE FUNKCJE (BEZ ZMIAN)
// =========================================================

void ProfileView::on_btnAddMeasurement_clicked()
{
    if (!currentUser) return;
    double weight = ui->spinWeight->value();
    int height = ui->spinHeight->value();
    QString date = QDate::currentDate().toString("yyyy-MM-dd");

    if (height == 0) return;
    double bmi = weight / ((height/100.0) * (height/100.0));

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery q(db);
    q.prepare("INSERT INTO user_measurements (user_id, weight, height, bmi, date) VALUES (:uid, :w, :h, :bmi, :date)");
    q.bindValue(":uid", currentUser->getId());
    q.bindValue(":w", weight);
    q.bindValue(":h", height);
    q.bindValue(":bmi", bmi);
    q.bindValue(":date", date);
    q.exec();

    // Update usera
    QSqlQuery qUp(db);
    qUp.prepare("UPDATE users SET weight_kg = :w, height_cm = :h WHERE id = :uid");
    qUp.bindValue(":w", weight);
    qUp.bindValue(":h", height);
    qUp.bindValue(":uid", currentUser->getId());
    qUp.exec();

    currentUser->setWeightKg(weight);
    currentUser->setHeightCm(height);

    QMessageBox::information(this, "Sukces", "Zapisano nowy pomiar!");
    loadMeasurementsHistory();
}

void ProfileView::loadMeasurementsHistory()
{
    if (!currentUser) return;
    ui->tableMeasurements->setRowCount(0);
    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT date, weight, height, bmi FROM user_measurements WHERE user_id = :uid ORDER BY date DESC");
    q.bindValue(":uid", currentUser->getId());

    if (q.exec()) {
        bool isLatest = true;
        while(q.next()) {
            int row = ui->tableMeasurements->rowCount();
            ui->tableMeasurements->insertRow(row);
            ui->tableMeasurements->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
            ui->tableMeasurements->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
            ui->tableMeasurements->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));

            double bmi = q.value(3).toDouble();
            ui->tableMeasurements->setItem(row, 3, new QTableWidgetItem(QString::number(bmi, 'f', 2)));

            if (isLatest) {
                ui->lblBMIResult->setText(QString("BMI: %1\n%2").arg(bmi, 0, 'f', 2).arg(getBMICategory(bmi)));
                ui->lblBMIResult->setStyleSheet(QString("background-color: %1; color: #1B1B2F; border: 1px solid gray; padding: 15px; border-radius: 8px; font-weight: bold;").arg(getBMIColor(bmi)));
                isLatest = false;
            }
        }
    }
}

void ProfileView::loadAvatar()
{
    if (!currentUser) return;
    QString path = QCoreApplication::applicationDirPath() + QString("/avatars/avatar_%1.png").arg(currentUser->getId());
    QPixmap pix(path);

    if (!pix.isNull()) {
        ui->lblAvatar->setPixmap(pix);
        ui->lblAvatar->setText("");
    } else {
        QPixmap defaultPic(":/rec/img/78851635318872.jpg");
        if (!defaultPic.isNull()) {
            int w = ui->lblAvatar->width();
            int h = ui->lblAvatar->height();
            ui->lblAvatar->setPixmap(defaultPic.scaled(w, h, Qt::KeepAspectRatio));
            ui->lblAvatar->setText("");
        } else {
            ui->lblAvatar->setText("Brak zdjęcia");
        }
    }
}

void ProfileView::on_btnChangePhoto_clicked()
{
    if (!currentUser) return;
    QString fileName = QFileDialog::getOpenFileName(this, "Wybierz zdjęcie", "", "Obrazy (*.png *.jpg *.jpeg)");
    if (fileName.isEmpty()) return;

    QString saveDir = QCoreApplication::applicationDirPath() + "/avatars";
    QDir dir(saveDir);
    if (!dir.exists()) dir.mkpath(".");

    QString savePath = saveDir + QString("/avatar_%1.png").arg(currentUser->getId());
    QPixmap pix(fileName);
    if (!pix.isNull()) {
        pix = pix.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pix.save(savePath, "PNG");
        ui->lblAvatar->setPixmap(pix);
        ui->lblAvatar->setText("");
    }
}

QString ProfileView::getBMICategory(double bmi)
{
    if (bmi < 18.5) return "Niedowaga";
    if (bmi < 25) return "Norma";
    return "Nadwaga";
}

QString ProfileView::getBMIColor(double bmi)
{
    if (bmi < 18.5) return "#fff9c4";
    if (bmi < 25) return "#c8e6c9";
    return "#ffcdd2";
}
