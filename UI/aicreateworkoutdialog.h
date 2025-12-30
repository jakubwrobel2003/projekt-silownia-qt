#ifndef AICREATEWORKOUTDIALOG_H
#define AICREATEWORKOUTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>

#include "AI/AIWorkoutGenerator.h"
#include "AI/AIWorkoutPlan.h"

class AICreateWorkoutDialog : public QDialog {
    Q_OBJECT
public:
    explicit AICreateWorkoutDialog(QWidget* parent = nullptr);

    bool hasApprovedPlan() const;
    AIWorkoutPlan approvedPlan() const;

private slots:
    void onGenerate();
    void onPlanReady(const AIWorkoutPlan& plan);
    void onError(const QString& msg);
    void onApprove();

private:
    void renderPlan(const AIWorkoutPlan& plan);

    // AI
    AIWorkoutGenerator generator;

    // UI
    QRadioButton* fbwRadio;
    QLineEdit* nameEdit;
    QListWidget* list;
    QPushButton* genBtn;
    QPushButton* approveBtn;
    QPushButton* rejectBtn;

    // state
    bool approved = false;
    AIWorkoutPlan plan;
};

#endif // AICREATEWORKOUTDIALOG_H
