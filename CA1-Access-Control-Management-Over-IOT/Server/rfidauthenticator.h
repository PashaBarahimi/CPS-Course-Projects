#ifndef RFIDAUTHENTICATOR_H
#define RFIDAUTHENTICATOR_H

#include <QObject>
#include <QString>

#include "rfidauthenticationitem.h"

namespace CPS {

class RfidAuthenticator : public QObject {
    Q_OBJECT
public:
    RfidAuthenticator(const QString& jsonFilePath);
    bool authenticate(const RfidAuthenticationItem& rfidAuthenticationItem);

Q_SIGNALS:
    void authenticated(RfidAuthenticationItem item);
    void unauthenticated(RfidAuthenticationItem item);

private:
    void LoadTags();

private:
    QString jsonFilePath_;
    QList<QString> rfidTags_;
};

} // namespace CPS

#endif // RFIDAUTHENTICATOR_H
