#ifndef RFIDAUTHENTICATIONITEM_H
#define RFIDAUTHENTICATIONITEM_H

#include <QJsonObject>
#include <QString>

namespace CPS {

class RfidAuthenticationItem {
public:
    RfidAuthenticationItem(const QString& username, const QString& time, const QString& date);
    RfidAuthenticationItem(const QJsonObject& jsonObject);
    RfidAuthenticationItem(const QString& username);
    QJsonObject toJsonObject() const;
    QString username() const;

private:
    QString username_;
    QString time_;
    QString date_;
};

} // namespace CPS

#endif // RFIDAUTHENTICATIONITEM_H
