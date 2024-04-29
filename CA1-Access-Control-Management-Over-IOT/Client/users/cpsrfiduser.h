#ifndef CPSRFIDUSER_H
#define CPSRFIDUSER_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

namespace CPS {

class RfidUser {
public:
    RfidUser(const QString& rfid, QDateTime datetime);
    RfidUser(const QString& rfid, const QString& date, const QString& time);
    QString rfid() const;
    QJsonObject toJsonObject() const;

private:
    QString rfid_;
    QDateTime datetime_;
};

} // namespace CPS

#endif // CPSRFIDUSER_H
