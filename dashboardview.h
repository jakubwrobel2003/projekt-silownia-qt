#ifndef DASHBOARDVIEW_H
#define DASHBOARDVIEW_H

#include <QWidget>

namespace Ui {
class DashboardView;
}

class DashboardView : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardView(QWidget *parent = nullptr);
    ~DashboardView();

private:
    Ui::DashboardView *ui;
};

#endif // DASHBOARDVIEW_H
