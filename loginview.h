#ifndef LOGINVIEW_H
#define LOGINVIEW_H
#include "./Repository/userrepository.h"
#include <QWidget>

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

signals:
    void loginSuccessful(UserModel* user); // Sygnał po udanym logowaniu

private slots:
   void onLoginButtonClicked();

private:
    UserModel* user = nullptr;
    Ui::LoginView *ui;  // Wskaźnik do UI
    UserRepository *userRepository;
};

#endif // LOGINVIEW_H
