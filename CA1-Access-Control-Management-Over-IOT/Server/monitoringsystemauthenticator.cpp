#include "monitoringsystemauthenticator.h"

#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils.h"

namespace CPS {

MonitoringSystemAuthenticator::MonitoringSystemAuthenticator(const QString& jsonFilePath)
    : jsonFilePath_(jsonFilePath) {
    loadUsers();
}

MonitoringSystemAuthenticator::~MonitoringSystemAuthenticator() {
    saveUsers();
}

bool MonitoringSystemAuthenticator::authenticate(const QString& username, const QString& password) {
    QString decodedPassword = decodePassword(password);
    QString hashedPassword = hashPassword(decodedPassword);
    for (const MonitoringSystemUser& user : users_) {
        if (user.username() == username && user.hashedPassword() == hashedPassword) {
            return true;
        }
    }
    return false;
}

void MonitoringSystemAuthenticator::addUser(const QString& username, const QString& password) {
    QString hashedPassword = hashPassword(password);
    MonitoringSystemUser user(username, hashedPassword);
    users_.append(user);
}

void MonitoringSystemAuthenticator::loadUsers() {
    QByteArray jsonFile = Utils::readFile(jsonFilePath_);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile);
    QJsonArray jsonArray = jsonDocument.array();

    for (const QJsonValue& jsonValue : jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        MonitoringSystemUser user(jsonObject);
        users_.append(user);
    }
}

void MonitoringSystemAuthenticator::saveUsers() {
    QJsonArray jsonArray;
    for (const MonitoringSystemUser& user : users_) {
        jsonArray.append(user.toJsonObject());
    }

    QJsonDocument jsonDocument(jsonArray);
    QByteArray jsonFile = jsonDocument.toJson();
    Utils::writeFile(jsonFilePath_, jsonFile);
}

QString MonitoringSystemAuthenticator::decodePassword(const QString& encodedPassword) {
    QByteArray decodedPassword = QByteArray::fromBase64(encodedPassword.toUtf8());
    return QString::fromUtf8(decodedPassword);
}

QString MonitoringSystemAuthenticator::hashPassword(const QString& password) {
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromUtf8(hashedPassword.toHex());
}

} // namespace CPS
