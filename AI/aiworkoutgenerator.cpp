#include "aiworkoutgenerator.h"

AIWorkoutGenerator::AIWorkoutGenerator(QObject* parent)
    : QObject(parent), gemini(this)
{
    connect(&gemini, &GeminiService::success,
            this, &AIWorkoutGenerator::onGeminiSuccess);

    connect(&gemini, &GeminiService::error,
            this, &AIWorkoutGenerator::onGeminiError);
}

void AIWorkoutGenerator::generateFBW(const QStringList& allowedExercises)
{
    QString exerciseList = allowedExercises.join(", ");

    QString prompt =
        "Generate a FULL BODY WORKOUT.\n"
        "You MUST use ONLY exercises from this list:\n"
        + exerciseList + "\n\n"
                         "DO NOT invent new exercises.\n"
                         "Return plain text, one exercise per line.\n\n"
                         "FORMAT:\n"
                         "Strength: Name; sets; reps; weight\n"
                         "Cardio: Name; duration(min); distance(km)\n\n"
                         "Example:\n"
                         "Strength: Bench Press; 4; 8; 60\n"
                         "Cardio: Treadmill Run; 20; 3\n";

    gemini.generate(prompt);
}

void AIWorkoutGenerator::onGeminiSuccess(const QString& text)
{
    AIWorkoutPlan plan;
    plan.name = "AI FBW";

    QStringList lines = text.split('\n', Qt::SkipEmptyParts);

    for (const QString& line : lines)
    {
        QString l = line.trimmed();

        if (l.startsWith("Strength:", Qt::CaseInsensitive))
        {
            QString data = l.mid(QString("Strength:").length()).trimmed();
            QStringList parts = data.split(';');

            if (parts.size() < 4) continue;

            AIExercise ex;
            ex.type = "strength";
            ex.name = parts[0].trimmed();
            ex.sets = parts[1].toInt();
            ex.reps = parts[2].toInt();
            ex.weight = parts[3].toDouble();

            plan.exercises.append(ex);
        }
        else if (l.startsWith("Cardio:", Qt::CaseInsensitive))
        {
            QString data = l.mid(QString("Cardio:").length()).trimmed();
            QStringList parts = data.split(';');

            if (parts.size() < 3) continue;

            AIExercise ex;
            ex.type = "cardio";
            ex.name = parts[0].trimmed();
            ex.duration = parts[1].toInt();
            ex.distance = parts[2].toDouble();

            plan.exercises.append(ex);
        }
    }

    if (plan.exercises.isEmpty())
    {
        emit error("AI nie zwróciło poprawnego planu");
        return;
    }

    emit planReady(plan);
}

void AIWorkoutGenerator::onGeminiError(const QString& message)
{
    emit error(message);
}
