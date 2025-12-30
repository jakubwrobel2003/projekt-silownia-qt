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
