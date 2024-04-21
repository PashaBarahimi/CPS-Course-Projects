#include "monitoringsystemauthenticationitem.h"

namespace CPS {

MonitoringSystemAuthenticationItem::MonitoringSystemAuthenticationItem(const QString& username, const QString& time, const QString& date)
    : username_(username), time_(time), date_(date) {}

MonitoringSystemAuthenticationItem::MonitoringSystemAuthenticationItem(const QJsonObject& jsonObject)
    : username_(jsonObject["username"].toString()), time_(jsonObject["time"].toString()), date_(jsonObject["date"].toString()) {}

QJsonObject MonitoringSystemAuthenticationItem::toJsonObject() const
{
    QJsonObject jsonObject;
    jsonObject["username"] = username_;
    jsonObject["time"] = time_;
    jsonObject["date"] = date_;
    return jsonObject;
}

} // namespace CPS
