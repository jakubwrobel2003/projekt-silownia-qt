#include "workoutsview.h"
#include "ui_workoutsview.h"

WorkoutsView::WorkoutsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WorkoutsView)
{
    ui->setupUi(this);
}

WorkoutsView::~WorkoutsView()
{
    delete ui;
}
