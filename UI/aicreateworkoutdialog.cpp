#include "AICreateWorkoutDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

AICreateWorkoutDialog::AICreateWorkoutDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Stwórz trening AI");
    resize(520, 420);
    setModal(true);

    fbwRadio = new QRadioButton("FBW (Full Body Workout) – na razie jedyna opcja", this);
    fbwRadio->setChecked(true);

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Nazwa treningu (uzupełniona po generacji)");

    list = new QListWidget(this);

    genBtn     = new QPushButton("Generuj plan", this);
    approveBtn = new QPushButton("Zatwierdź (zapisz do bazy)", this);
    rejectBtn  = new QPushButton("Odrzuć", this);
    approveBtn->setEnabled(false);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(genBtn);
    buttons->addWidget(approveBtn);
    buttons->addWidget(rejectBtn);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->addWidget(fbwRadio);
    root->addWidget(nameEdit);
    root->addWidget(list);
    root->addLayout(buttons);

    connect(genBtn, &QPushButton::clicked, this, &AICreateWorkoutDialog::onGenerate);
    connect(approveBtn, &QPushButton::clicked, this, &AICreateWorkoutDialog::onApprove);
    connect(rejectBtn, &QPushButton::clicked, this, &QDialog::reject);

    connect(&generator, &AIWorkoutGenerator::planReady, this, &AICreateWorkoutDialog::onPlanReady);
    connect(&generator, &AIWorkoutGenerator::error, this, &AICreateWorkoutDialog::onError);
}

void AICreateWorkoutDialog::onGenerate()
{
    approveBtn->setEnabled(false);
    list->clear();
    nameEdit->clear();
    generator.generateFBW();
}

void AICreateWorkoutDialog::onPlanReady(const AIWorkoutPlan& p)
{
    plan = p;
    renderPlan(plan);
    nameEdit->setText(plan.workoutName);
    approveBtn->setEnabled(true);
}

void AICreateWorkoutDialog::onError(const QString& msg)
{
    QMessageBox::warning(this, "AI error", msg);
}

void AICreateWorkoutDialog::renderPlan(const AIWorkoutPlan& p)
{
    list->clear();
    for (const auto& e : p.exercises) {
        list->addItem(
            QString("%1 (%2)  %3x%4")
                .arg(e.name)
                .arg(e.type)
                .arg(e.sets)
                .arg(e.reps)
            );
    }
}

void AICreateWorkoutDialog::onApprove()
{
    if (plan.exercises.isEmpty()) {
        QMessageBox::warning(this, "Brak planu", "Najpierw wygeneruj plan.");
        return;
    }
    plan.workoutName = nameEdit->text().trimmed();
    if (plan.workoutName.isEmpty())
        plan.workoutName = "FBW - AI Generated";

    approved = true;
    accept();
}

bool AICreateWorkoutDialog::hasApprovedPlan() const { return approved; }
AIWorkoutPlan AICreateWorkoutDialog::approvedPlan() const { return plan; }
