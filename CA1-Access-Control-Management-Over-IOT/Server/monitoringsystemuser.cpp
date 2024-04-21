#include "monitoringsystemuser.h"

namespace CPS {

MonitoringSystemUser::MonitoringSystemUser(const QString& username, const QString& hashedPassword)
    : username_(username)
    , hashedPassword_(hashedPassword) {}

QString MonitoringSystemUser::username() const
{
    return username_;
}

QString MonitoringSystemUser::hashedPassword() const
{
    return hashedPassword_;
}

} // namespace CPS
