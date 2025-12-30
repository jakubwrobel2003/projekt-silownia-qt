#ifndef GEMINISERVICE_H
#define GEMINISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GeminiService : public QObject {
    Q_OBJECT
public:
    explicit GeminiService(QObject* parent = nullptr);

    void generate(const QString& prompt);

signals:
    void success(const QString& responseText);
    void error(const QString& message);

private slots:
    void onFinished(QNetworkReply* reply);

private:
    QString readApiKey() const;
    QNetworkAccessManager manager;
};

#endif // GEMINISERVICE_H
