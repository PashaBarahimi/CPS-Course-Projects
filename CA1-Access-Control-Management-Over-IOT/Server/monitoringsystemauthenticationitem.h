#ifndef MONITORINGSYSTEMAUTHENTICATIONITEM_H
#define MONITORINGSYSTEMAUTHENTICATIONITEM_H

#include <QString>
#include <QJsonObject>

namespace CPS {

class MonitoringSystemAuthenticationItem
{
public:
    MonitoringSystemAuthenticationItem(const QString& username, const QString& time, const QString& date);
    MonitoringSystemAuthenticationItem(const QJsonObject& jsonObject);
    QJsonObject toJsonObject() const;

private:
    QString username_;
    QString time_;
    QString date_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMAUTHENTICATIONITEM_H
