#include "PasswordHasher.h"
#include <QCryptographicHash>

QString PasswordHasher::hash(const QString& password) {
    return QString(
        QCryptographicHash::hash(
            password.toUtf8(),
            QCryptographicHash::Sha256
            ).toHex()
        );
}
