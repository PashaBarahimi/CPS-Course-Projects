#include "cpsuser.h"

namespace CPS {

User::User(const QString& username, const QString& password)
    : username_(username),
      password_(password) {}

QString User::username() const { return username_; }
QString User::password() const { return password_; }

QJsonObject User::toJsonObject(bool base64Password) const {
    QJsonObject json;
    json["username"] = username_;
    if (base64Password) {
        json["password"] = QString::fromUtf8(password_.toUtf8().toBase64());
    }
    else {
        json["password"] = password_;
    }
    return json;
}

} // namespace CPS
