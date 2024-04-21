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
    ~MonitoringSystemAuthenticator();
    bool authenticate(const QString& username, const QString& password);
    void addUser(const QString& username, const QString& password);

private:
    void loadUsers();
    void saveUsers();
    QString decodePassword(const QString& encodedPassword);
    QString hashPassword(const QString& password);

private:
    QString jsonFilePath_;
    QList<MonitoringSystemUser> users_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMAUTHENTICATOR_H
