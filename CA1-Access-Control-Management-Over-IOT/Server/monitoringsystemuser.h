#ifndef MONITORINGSYSTEMUSER_H
#define MONITORINGSYSTEMUSER_H

#include <QJsonObject>
#include <QString>

namespace CPS {

class MonitoringSystemUser {
public:
    MonitoringSystemUser(const QString& username, const QString& hashedPassword);
    MonitoringSystemUser(const QJsonObject& jsonObject);
    QString username() const;
    QString hashedPassword() const;
    QJsonObject toJsonObject() const;

private:
    QString username_;
    QString hashedPassword_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMUSER_H
