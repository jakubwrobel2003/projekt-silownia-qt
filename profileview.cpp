#include "profileview.h"
#include "ui_profileview.h"

ProfileView::ProfileView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfileView)
{
    ui->setupUi(this);
}

ProfileView::~ProfileView()
{
    delete ui;
}
void ProfileView::setUserData(UserModel* currentUser){
    this->currentUser=currentUser;
    ui->LoginLabel->setText("Login: "+currentUser->getLogin());
    ui->birthDateLabel->setText("Data urodzin: " + currentUser->getBirthDate().toString("yyyy-MM-dd"));
    ui->heightCmLabel->setText("Wzrost: " + QString::number(currentUser->getHeightCm()) + " cm");

  ui->weightKgLabel->setText("Waga: " + QString::number(currentUser->getWeightKg()));

}
