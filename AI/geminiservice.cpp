#include "GeminiService.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QJsonArray>
GeminiService::GeminiService(QObject* parent)
    : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished,
            this, &GeminiService::onFinished);
}

QString GeminiService::readApiKey() const
{
    QFile file("config.json");
    if (!file.open(QIODevice::ReadOnly))
        return {};

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return {};

    return doc.object().value("gemini_api_key").toString();
}

void GeminiService::generate(const QString& prompt)
{
    QString apiKey = readApiKey();
    if (apiKey.isEmpty()) {
        emit error("Brak klucza API (config.json)");
        return;
    }

    QUrl url(
        "https://generativelanguage.googleapis.com/v1beta/models/"
        "gemini-2.5-flash:generateContent?key=" + apiKey
        );

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("User-Agent", "QtGeminiClient/1.0");

    QJsonObject part;
    part["text"] = prompt;

    QJsonArray parts;
    parts.append(part);

    QJsonObject content;
    content["parts"] = parts;

    QJsonArray contents;
    contents.append(content);

    QJsonObject root;
    root["contents"] = contents;

    QJsonDocument doc(root);
    manager.post(req, doc.toJson());
}


void GeminiService::onFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    QJsonArray candidates = doc.object().value("candidates").toArray();
    if (candidates.isEmpty()) {
        emit error("Brak candidates w odpowiedzi AI");
        return;
    }

    QJsonObject content = candidates[0].toObject()
                              .value("content").toObject();
    QJsonArray parts = content.value("parts").toArray();

    if (parts.isEmpty()) {
        emit error("Brak parts w odpowiedzi AI");
        return;
    }

    QString text = parts[0].toObject().value("text").toString();
    emit success(text);
}
