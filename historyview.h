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

public:
    explicit HistoryView(QWidget *parent = nullptr);
    ~HistoryView();
    void setUserData(UserModel *currentUser);

public slots:
    void loadHistory(); // [NOWE]

private:
    Ui::HistoryView *ui;
    UserModel *currentUser = nullptr;
};

#endif // HISTORYVIEW_H
