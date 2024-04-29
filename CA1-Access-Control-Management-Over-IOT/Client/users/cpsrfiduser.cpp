#include "cpsrfiduser.h"

namespace CPS {

RfidUser::RfidUser(const QString& rfid, QDateTime datetime)
    : rfid_(rfid),
      datetime_(datetime) {}

RfidUser::RfidUser(const QString& rfid, const QString& date, const QString& time)
    : rfid_(rfid) {
    datetime_ = QDateTime::fromString(date + " " + time, "MM/dd/yyyy hh:mm");
    if (!datetime_.isValid()) {
        qDebug() << "Invalid datetime for rfid:" << rfid;
    }
}

QString RfidUser::rfid() const { return rfid_; }

QJsonObject RfidUser::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject["username"] = rfid_;
    jsonObject["date"] = datetime_.toString("MM/dd/yyyy");
    jsonObject["time"] = datetime_.toString("hh:mm");
    return jsonObject;
}

} // namespace CPS
