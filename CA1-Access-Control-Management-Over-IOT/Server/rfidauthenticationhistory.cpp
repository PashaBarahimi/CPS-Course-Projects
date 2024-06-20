#include "rfidauthenticationhistory.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils.h"

namespace CPS {

RfidAuthenticationHistory::RfidAuthenticationHistory(const QString& historyFilePath) {
    historyFilePath_ = historyFilePath;
    loadHistory();
}

RfidAuthenticationHistory::~RfidAuthenticationHistory() { saveHistory(); }

void RfidAuthenticationHistory::addItem(const RfidAuthenticationItem& item) {
    history_ << item;
}

void RfidAuthenticationHistory::historyRequested() {
    QJsonArray array;
    for (const auto& item : history_) {
        array << item.toJsonObject();
    }

    Q_EMIT historyReady(array);
}

void RfidAuthenticationHistory::loadHistory() {
    history_.clear();
    qDebug() << "Loading history from" << historyFilePath_;

    QByteArray data = Utils::readFile(historyFilePath_);
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (const auto& item : array) {
        RfidAuthenticationItem authItem(item.toObject());
        history_ << authItem;
    }
}

void RfidAuthenticationHistory::saveHistory() {
    qDebug() << "Saving history to" << historyFilePath_;
    QJsonArray array;
    for (const auto& item : history_) {
        array << item.toJsonObject();
    }

    QJsonDocument doc(array);
    QByteArray data = doc.toJson();
    Utils::writeFile(historyFilePath_, data);
}

} // namespace CPS
