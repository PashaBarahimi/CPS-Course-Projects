#include "monitoringsystemauthenticator.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>

#include "utils.h"

namespace CPS {

MonitoringSystemAuthenticator::MonitoringSystemAuthenticator(const QString& jsonFilePath)
    : jsonFilePath_(jsonFilePath)
    , users_(readUsers()) {}

bool MonitoringSystemAuthenticator::authenticate(const QString& username, const QString& password)
{
    QString decodedPassword = decodePassword(password);
    QString hashedPassword = hashPassword(decodedPassword);
    for (const MonitoringSystemUser& user : users_) {
        if (user.username() == username && user.hashedPassword() == hashedPassword) {
            return true;
        }
    }
    return false;
}

QList<MonitoringSystemUser> MonitoringSystemAuthenticator::readUsers()
{
    QByteArray jsonFile = Utils::readFile(jsonFilePath_);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile);
    QJsonArray jsonArray = jsonDocument.array();

    QList<MonitoringSystemUser> users;
    for (const QJsonValue& jsonValue : jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        QString username = jsonObject["username"].toString();
        QString hashedPassword = jsonObject["password"].toString();
        users.append(MonitoringSystemUser(username, hashedPassword));
    }
    return users;
}

QString MonitoringSystemAuthenticator::decodePassword(const QString& encodedPassword)
{
    QByteArray decodedPassword = QByteArray::fromBase64(encodedPassword.toUtf8());
    return QString::fromUtf8(decodedPassword);
}

QString MonitoringSystemAuthenticator::hashPassword(const QString& password)
{
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromUtf8(hashedPassword.toHex());
}

} // namespace CPS
