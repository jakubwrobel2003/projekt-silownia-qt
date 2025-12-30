#ifndef WORKOUTSVIEW_H
#define WORKOUTSVIEW_H

#include <QWidget>

namespace Ui {
class WorkoutsView;
}

class WorkoutsView : public QWidget
{
    Q_OBJECT

public:
    explicit WorkoutsView(QWidget *parent = nullptr);
    ~WorkoutsView();

private:
    Ui::WorkoutsView *ui;
};

#endif // WORKOUTSVIEW_H
