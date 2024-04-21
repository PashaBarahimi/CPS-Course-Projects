#include "monitoringsystemuser.h"

namespace CPS {

MonitoringSystemUser::MonitoringSystemUser(const QString& username, const QString& hashedPassword)
    : username_(username), hashedPassword_(hashedPassword) {}

MonitoringSystemUser::MonitoringSystemUser(const QJsonObject& jsonObject) {
    username_ = jsonObject["username"].toString();
    hashedPassword_ = jsonObject["hashedPassword"].toString();
}

QString MonitoringSystemUser::username() const {
    return username_;
}

QString MonitoringSystemUser::hashedPassword() const {
    return hashedPassword_;
}

QJsonObject MonitoringSystemUser::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject["username"] = username_;
    jsonObject["hashedPassword"] = hashedPassword_;
    return jsonObject;
}

} // namespace CPS
