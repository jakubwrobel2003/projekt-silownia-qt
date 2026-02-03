#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QWidget>
#include "./Model/usermodel.h"
namespace Ui {
class HistoryView;
}

class HistoryView : public QWidget
{
    Q_OBJECT
    UserModel *currentUser;

public:
    explicit HistoryView(QWidget *parent = nullptr);
    ~HistoryView();
    void setUserData(UserModel *currentUser);

private:
    Ui::HistoryView *ui;
};

#endif // HISTORYVIEW_H
