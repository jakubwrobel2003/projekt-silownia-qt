#include "LoginView.h"
#include "ui_LoginView.h"
#include "./Repository/userrepository.h"
#include "./Model/usermodel.h"
LoginView::LoginView(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginView), userRepository(new UserRepository())  // Initialize userRepository
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginView::onLoginButtonClicked);

}


LoginView::~LoginView() {
    delete ui;
    delete userRepository;  // Delete userRepository in the destructor
}

void LoginView::onLoginButtonClicked() {
    QString username = ui->usernameField->text();
    QString password = ui->passwordField->text();

    // Teraz loginValid zwraca wskaźnik na obiekt UserModel
    UserModel* user = userRepository->loginValid(username, password);

    if (user) {
        // Emitowanie sygnału loginSuccessful i przekazanie obiektu UserModel
        emit loginSuccessful(user);
    } else {
        ui->errorLabel->setText("Invalid username or password.");
    }
}

