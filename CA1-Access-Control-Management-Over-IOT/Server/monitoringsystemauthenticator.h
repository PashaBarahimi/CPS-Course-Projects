#ifndef MONITORINGSYSTEMAUTHENTICATOR_H
#define MONITORINGSYSTEMAUTHENTICATOR_H

#include <QString>
#include <QList>
#include <QJsonArray>

#include "monitoringsystemuser.h"

namespace CPS {

class MonitoringSystemAuthenticator
{
public:
    MonitoringSystemAuthenticator(const QString& jsonFilePath);
    bool authenticate(const QString& username, const QString& password);

private:
    QList<MonitoringSystemUser> readUsers();
    QString decodePassword(const QString& encodedPassword);
    QString hashPassword(const QString& password);

private:
    QString jsonFilePath_;
    QList<MonitoringSystemUser> users_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMAUTHENTICATOR_H
