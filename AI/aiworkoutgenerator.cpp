#include "AIWorkoutGenerator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AIWorkoutGenerator::AIWorkoutGenerator(QObject* parent)
    : QObject(parent), gemini(this)
{
    connect(&gemini, &GeminiService::success, this,
            [this](const QString& text) {
                AIWorkoutPlan plan;
                QString err;
                if (!parseResponse(text, plan, err)) {
                    emit error(err);
                    return;
                }
                emit planReady(plan);
            });

    connect(&gemini, &GeminiService::error,
            this, &AIWorkoutGenerator::error);
}

void AIWorkoutGenerator::generateFBW()
{
    gemini.generate(buildPrompt());
}

QString AIWorkoutGenerator::buildPrompt() const
{
    return
        "Generate FULL BODY WORKOUT (FBW).\n"
        "Use ONLY these exercise IDs:\n"
        "1 Bench Press\n"
        "2 Squat\n"
        "3 Deadlift\n"
        "4 Lat Pulldown\n"
        "5 Shoulder Press\n"
        "6 Biceps Curl\n"
        "7 Triceps Pushdown\n"
        "8 Plank\n"
        "\n"
        "Rules:\n"
        "- exactly 6 exercises\n"
        "- output JSON ONLY\n"
        "- format:\n"
        "{\n"
        "  \"workout_name\": \"FBW - AI\",\n"
        "  \"exercises\": [\n"
        "    {\"exercise_definition_id\":1,\"sets\":3,\"reps\":\"8-10\"}\n"
        "  ]\n"
        "}";
}

static QString extractJson(const QString& text)
{
    int a = text.indexOf('{');
    int b = text.lastIndexOf('}');
    if (a < 0 || b < 0 || b <= a) return {};
    return text.mid(a, b - a + 1);
}

bool AIWorkoutGenerator::parseResponse(
    const QString& text,
    AIWorkoutPlan& plan,
    QString& err)
{
    QString jsonText = extractJson(text);
    if (jsonText.isEmpty()) {
        err = "Brak JSON w odpowiedzi AI";
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    if (!doc.isObject()) {
        err = "Niepoprawny JSON";
        return false;
    }

    QJsonObject root = doc.object();
    plan.workoutName = root.value("workout_name").toString("FBW - AI");

    QJsonArray arr = root.value("exercises").toArray();
    if (arr.size() != 6) {
        err = "AI zwróciło złą liczbę ćwiczeń";
        return false;
    }

    plan.exercises.clear();
    for (auto v : arr) {
        QJsonObject o = v.toObject();
        AIWorkoutExercise e;
        e.exerciseDefinitionId = o.value("exercise_definition_id").toInt();
        e.sets = o.value("sets").toInt();
        e.reps = o.value("reps").toString();

        if (e.exerciseDefinitionId <= 0 || e.sets <= 0 || e.reps.isEmpty()) {
            err = "Niepoprawne dane ćwiczenia";
            return false;
        }
        plan.exercises.append(e);
    }
    return true;
}
