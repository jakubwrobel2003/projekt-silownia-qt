#include "LoginView.h"
#include "./Model/usermodel.h"
#include "./Repository/userrepository.h"
#include "ui_LoginView.h"
#include <QPixmap>       // <-- Potrzebne do logo
#include <QMessageBox>   // <-- Potrzebne do komunikatów błędów

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
    , userRepository(new UserRepository())
{
    ui->setupUi(this);

    // =========================================================
    // 🖼️ ŁADOWANIE LOGO
    // =========================================================
    QPixmap log(":/new/logo/MacroMind logo with brain and barbell.png");
    if (!log.isNull()) {
        // Skalujemy, żeby pasowało do labela w UI
        ui->lblLogo->setPixmap(log.scaled(400, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // Fallback, gdyby grafika się nie załadowała
        ui->lblLogo->setText("MacroMind Gym Tracker");
    }

    // =========================================================
    // 🔗 POŁĄCZENIA SYGNAŁÓW
    // =========================================================
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginView::onLoginButtonClicked);

    // Dodatkowo: Logowanie po naciśnięciu ENTER w polu hasła
    connect(ui->passwordInput, &QLineEdit::returnPressed, this, &LoginView::onLoginButtonClicked);
}

LoginView::~LoginView()
{
    delete ui;
    delete userRepository;
}

void LoginView::onLoginButtonClicked()
{
    // Uwaga: W nowym pliku .ui pola nazywają się 'loginInput' i 'passwordInput'
    QString username = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    // Szybkie sprawdzenie czy pola nie są puste
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wpisać login i hasło.");
        return;
    }

    // Sprawdzenie w repozytorium
    UserModel *user = userRepository->loginValid(username, password);

    if (user) {
        // Sukces - emitujemy sygnał do MainWindow
        emit loginSuccessful(user);
    } else {
        // Błąd - wyświetlamy okienko (bo usunęliśmy errorLabel dla czystości designu)
        QMessageBox::warning(this, "Błąd logowania", "Nieprawidłowa nazwa użytkownika lub hasło.");
    }
}
