#ifndef CPSUSER_H
#define CPSUSER_H

#include <QJsonObject>
#include <QString>

namespace CPS {

class User {
public:
    User(const QString& username, const QString& password);
    QString username() const;
    QString password() const;
    QJsonObject toJsonObject(bool base64Password = true) const;

private:
    QString username_;
    QString password_;
};

} // namespace CPS

#endif // CPSUSER_H
