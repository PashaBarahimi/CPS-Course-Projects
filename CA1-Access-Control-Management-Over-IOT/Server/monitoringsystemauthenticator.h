#ifndef MONITORINGSYSTEMAUTHENTICATOR_H
#define MONITORINGSYSTEMAUTHENTICATOR_H

#include <QJsonArray>
#include <QList>
#include <QObject>
#include <QString>

#include "monitoringsystemuser.h"

namespace CPS {

class MonitoringSystemAuthenticator : public QObject {
    Q_OBJECT
public:
    MonitoringSystemAuthenticator(const QString& jsonFilePath);
    ~MonitoringSystemAuthenticator();
    void addUser(const QString& username, const QString& password);

Q_SIGNALS:
    void authenticated();
    void unauthenticated();

public Q_SLOTS:
    void authenticate(const QString& username, const QString& password);

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
