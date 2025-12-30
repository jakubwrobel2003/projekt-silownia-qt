#include "DashboardView.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

DashboardView::DashboardView(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(12);

    auto* h1 = new QLabel("Dashboard", this);
    h1->setStyleSheet("font-size: 22px; font-weight: 800;");
    layout->addWidget(h1);

    auto* info = new QLabel(
        "Tu będzie skrót: ostatni trening, liczba sesji, szybkie akcje.",
        this
        );
    info->setStyleSheet("color: #A8A8A8;");
    layout->addWidget(info);

    auto* card = new QFrame(this);
    card->setStyleSheet("background:#1A1A1A; border:1px solid #2A2A2A; border-radius:14px;");
    auto* cardL = new QVBoxLayout(card);
    cardL->setContentsMargins(16, 16, 16, 16);

    auto* t = new QLabel("Ostatnia sesja: —", card);
    t->setStyleSheet("font-size:16px; font-weight:700;");
    cardL->addWidget(t);

    auto* t2 = new QLabel("Objętość / czas / PR-y: —", card);
    t2->setStyleSheet("color:#A8A8A8;");
    cardL->addWidget(t2);

    layout->addWidget(card);
    layout->addStretch(1);
}
