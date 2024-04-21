#include <QCoreApplication>

#include "monitoringsystemauthenticator.h"

const QString MONITORING_SYSTEM_USERS_JSON_FILE_PATH = "data/monitoring_system_users.json";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CPS::MonitoringSystemAuthenticator monitoringSystemAuthenticator(MONITORING_SYSTEM_USERS_JSON_FILE_PATH);

    return a.exec();
}
