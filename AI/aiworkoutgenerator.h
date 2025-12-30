#ifndef AIWORKOUTGENERATOR_H
#define AIWORKOUTGENERATOR_H

#include <QObject>
#include "AIWorkoutPlan.h"
#include "GeminiService.h"

class AIWorkoutGenerator : public QObject {
    Q_OBJECT
public:
    explicit AIWorkoutGenerator(QObject* parent = nullptr);

    void generateFBW();

signals:
    void planReady(const AIWorkoutPlan& plan);
    void error(const QString& message);

private:
    QString buildPrompt() const;
    bool parseResponse(const QString& text, AIWorkoutPlan& plan, QString& err);

    GeminiService gemini;
};

#endif // AIWORKOUTGENERATOR_H
