#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H
#include <QWidget>
#include "./Model/usermodel.h"

namespace Ui {
class ProfileView;
}

class ProfileView : public QWidget
{
    Q_OBJECT

public:
    explicit ProfileView(QWidget *parent = nullptr);
    ~ProfileView();

    void setUserData(UserModel *currentUser);

private:
    Ui::ProfileView *ui;
    UserModel *currentUser = nullptr;
};

#endif // PROFILEVIEW_H
