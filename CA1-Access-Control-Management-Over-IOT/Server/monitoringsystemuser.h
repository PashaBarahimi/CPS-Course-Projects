#ifndef MONITORINGSYSTEMUSER_H
#define MONITORINGSYSTEMUSER_H

#include <QString>

namespace CPS {

class MonitoringSystemUser
{
public:
    MonitoringSystemUser(const QString& username, const QString& hashedPassword);
    QString username() const;
    QString hashedPassword() const;

private:
    QString username_;
    QString hashedPassword_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMUSER_H
