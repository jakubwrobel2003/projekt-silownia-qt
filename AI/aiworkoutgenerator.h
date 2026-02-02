#ifndef AIWORKOUTGENERATOR_H
#define AIWORKOUTGENERATOR_H

#include <QObject>
#include "aiworkoutplan.h"
#include "GeminiService.h"

class AIWorkoutGenerator : public QObject
{
    Q_OBJECT
public:
    explicit AIWorkoutGenerator(QObject* parent = nullptr);

   void generateFBW(const QStringList& allowedExercises); // uruchamia Gemini

signals:
    void planReady(const AIWorkoutPlan& plan);
    void error(const QString& message);

private slots:
    void onGeminiSuccess(const QString& text);
    void onGeminiError(const QString& message);

private:
    GeminiService gemini;
};

#endif
