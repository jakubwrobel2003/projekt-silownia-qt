#include "helpview.h"
#include "ui_helpview.h"

HelpView::HelpView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HelpView)
{
    ui->setupUi(this);

    // Treść pomocy w HTML - Pasująca do motywu Dark Navy
    QString helpText = R"(
    <body style='color: #E0E0E0; font-family: Segoe UI; font-size: 15px; background-color: #232336;'>

        <h2 style='color: #4CAF50;'>Witaj w MacroMind Gym Tracker! 🏋️‍♂️</h2>
        <p>Twoja kompletna aplikacja do śledzenia postępów na siłowni.</p>

        <hr style='border: 1px solid #4B4B6E;'>

        <h3 style='color: #00E5FF;'>1. Workouts (Kreator Planów)</h3>
        <p>Zanim zaczniesz ćwiczyć, musisz mieć plan.</p>
        <ul>
            <li>Wpisz nazwę treningu (np. "Klatka + Biceps").</li>
            <li>Wybieraj ćwiczenia z listy i klikaj <b>Dodaj</b>.</li>
            <li>Możesz też użyć przycisku <b>Generuj AI</b>, aby sztuczna inteligencja ułożyła plan za Ciebie.</li>
            <li>Na końcu kliknij <b>Zapisz Plan</b>.</li>
        </ul>

        <h3 style='color: #00E5FF;'>2. Trening (Realizacja)</h3>
        <p>To Twój cyfrowy notes na siłownię.</p>
        <ul>
            <li>Po lewej stronie wybierz plan, który chcesz dziś wykonać.</li>
            <li>Po prawej zobaczysz listę ćwiczeń.</li>
            <li>Wpisz <b>faktycznie wykonane</b> powtórzenia i ciężar w polach edycji.</li>
            <li>Kliknij zielony przycisk <b>ZAPISZ TRENING</b>, aby dodać go do historii.</li>
        </ul>

        <h3 style='color: #00E5FF;'>3. Pulpit (Dashboard)</h3>
        <p>Centrum analityczne Twoich postępów.</p>
        <ul>
            <li><b>Kalendarz:</b> Dni, w których trenowałeś, są zaznaczone na zielono.</li>
            <li><b>Analiza Siły:</b> Wybierz ćwiczenie z listy, aby zobaczyć wykres progresu ciężaru.</li>
            <li><b>Rekord 1RM:</b> Aplikacja automatycznie oblicza Twój teoretyczny rekord siłowy (maksymalny ciężar na 1 powtórzenie) na podstawie Twojej historii.</li>
        </ul>

        <h3 style='color: #00E5FF;'>4. Profil</h3>
        <p>Zarządzanie Twoim ciałem.</p>
        <ul>
            <li>Aktualizuj wagę i wzrost, aby śledzić BMI.</li>
            <li>Użyj <b>Kalkulatora Kalorii</b>, aby obliczyć swoje zapotrzebowanie (BMR/TDEE) w zależności od celu (Masa/Redukcja).</li>
            <li>Możesz tu również zmienić swoje zdjęcie profilowe.</li>
        </ul>

        <hr style='border: 1px solid #4B4B6E;'>
        <p style='color: gray; font-size: 13px; text-align: center;'>MacroMind Gym Tracker v1.0 | Created by wojfer87</p>
    </body>
    )";

    ui->textBrowser->setHtml(helpText);
}

HelpView::~HelpView()
{
    delete ui;
}
