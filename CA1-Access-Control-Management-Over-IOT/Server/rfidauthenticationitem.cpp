#include "rfidauthenticationitem.h"

#include <QDateTime>

namespace CPS {

RfidAuthenticationItem::RfidAuthenticationItem(const QString& username, const QString& time, const QString& date)
    : username_(username), time_(time), date_(date) {}

RfidAuthenticationItem::RfidAuthenticationItem(const QJsonObject& jsonObject)
    : username_(jsonObject["username"].toString()),
      time_(jsonObject["time"].toString()),
      date_(jsonObject["date"].toString()) {}

RfidAuthenticationItem::RfidAuthenticationItem(const QString& username)
    : username_(username),
      time_(QDateTime::currentDateTime().toString("hh:mm")),
      date_(QDateTime::currentDateTime().toString("MM/dd/yyyy")) {}

QJsonObject RfidAuthenticationItem::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject["username"] = username_;
    jsonObject["time"] = time_;
    jsonObject["date"] = date_;
    return jsonObject;
}

QString RfidAuthenticationItem::username() const { return username_; }

} // namespace CPS
