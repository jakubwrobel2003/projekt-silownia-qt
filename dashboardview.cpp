#include "dashboardview.h"
#include "ui_dashboardview.h"

DashboardView::DashboardView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardView)
{
    ui->setupUi(this);
}

DashboardView::~DashboardView()
{
    delete ui;
}
