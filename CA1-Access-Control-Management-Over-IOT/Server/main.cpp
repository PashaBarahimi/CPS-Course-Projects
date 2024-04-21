#include <QCoreApplication>

#include "monitoringsystemauthenticator.h"
#include "monitoringsystemhistory.h"

const QString MONITORING_SYSTEM_USERS_JSON_FILE_PATH = "data/monitoring_system_users.json";
const QString MONITORING_SYSTEM_HISTORY_JSON_FILE_PATH = "data/monitoring_system_history.json";

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    auto monitoringSystemAuthenticator = new CPS::MonitoringSystemAuthenticator(MONITORING_SYSTEM_USERS_JSON_FILE_PATH);
    auto monitoringSystemHistory = new CPS::MonitoringSystemHistory(MONITORING_SYSTEM_HISTORY_JSON_FILE_PATH);

    delete monitoringSystemAuthenticator;
    delete monitoringSystemHistory;

    return a.exec();
}
