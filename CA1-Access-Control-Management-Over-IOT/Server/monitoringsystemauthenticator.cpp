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

MonitoringSystemAuthenticator::~MonitoringSystemAuthenticator() { saveUsers(); }

void MonitoringSystemAuthenticator::authenticate(const QString& username, const QString& password) {
    QString decodedPassword = decodePassword(password);
    QString hashedPassword = hashPassword(decodedPassword);
    for (const MonitoringSystemUser& user : users_) {
        if (user.username() == username &&
            user.hashedPassword() == hashedPassword) {
            qDebug() << "User" << username << "authenticated";
            emit authenticated();
            return;
        }
    }
    qDebug() << "User" << username << "not authenticated";
    emit unauthenticated();
}

void MonitoringSystemAuthenticator::addUser(const QString& username, const QString& password) {
    QString hashedPassword = hashPassword(password);
    MonitoringSystemUser user(username, hashedPassword);
    users_ << user;
    qDebug() << "User" << username << "added";
}

void MonitoringSystemAuthenticator::loadUsers() {
    qDebug() << "Loading users from" << jsonFilePath_;
    QByteArray jsonFile = Utils::readFile(jsonFilePath_);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile);
    QJsonArray jsonArray = jsonDocument.array();

    for (const QJsonValue& jsonValue : jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        MonitoringSystemUser user(jsonObject);
        users_ << user;
        qDebug() << "User" << user.username() << "loaded";
    }
}

void MonitoringSystemAuthenticator::saveUsers() {
    qDebug() << "Saving users to" << jsonFilePath_;
    QJsonArray jsonArray;
    for (const MonitoringSystemUser& user : users_) {
        jsonArray << user.toJsonObject();
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
