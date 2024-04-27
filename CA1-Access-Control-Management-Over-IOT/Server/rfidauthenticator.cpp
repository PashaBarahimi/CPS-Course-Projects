#include "rfidauthenticator.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils.h"

namespace CPS {

RfidAuthenticator::RfidAuthenticator(const QString& jsonFilePath)
    : jsonFilePath_(jsonFilePath) {
    LoadTags();
}

bool RfidAuthenticator::authenticate(const RfidAuthenticationItem& rfidAuthenticationItem) {
    if (rfidTags_.contains(rfidAuthenticationItem.username())) {
        emit authenticated(rfidAuthenticationItem);
        return true;
    }
    else {
        emit unauthenticated(rfidAuthenticationItem);
        return false;
    }
}

void RfidAuthenticator::LoadTags() {
    qDebug() << "Loading tags from" << jsonFilePath_;
    QByteArray jsonFile = Utils::readFile(jsonFilePath_);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile);
    QJsonArray array = jsonDocument.array();

    for (const QJsonValue& value : array) {
        rfidTags_ << value.toString();
        qDebug() << "Tag" << value.toString() << "loaded";
    }
}

} // namespace CPS
