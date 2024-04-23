#include <QCoreApplication>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QLoggingCategory>

#include "monitoringsystemauthenticator.h"
#include "rfidauthenticationhistory.h"
#include "websocketserver.h"

const QString MONITORING_SYSTEM_USERS_JSON_FILE_PATH = "data/monitoring_system_users.json";
const QString RFID_AUTHENTICATION_HISTORY_JSON_FILE_PATH = "data/monitoring_system_history.json";

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    qSetMessagePattern("%{time} [%{type}] %{appname}: %{message}");
    QLoggingCategory::setFilterRules("*.debug=true");

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption webSocketPortOption("websocket_port", "Port for WebSocket server to listen on", "websocket_port", "12345");
    parser.addOption(webSocketPortOption);
    parser.process(a);

    int webSocketPort = parser.value(webSocketPortOption).toInt();

    qDebug() << "Starting CPS Server";

    auto monitoringSystemAuthenticator = new CPS::MonitoringSystemAuthenticator(MONITORING_SYSTEM_USERS_JSON_FILE_PATH);
    auto rfidAuthenticationHistory = new CPS::RfidAuthenticationHistory(RFID_AUTHENTICATION_HISTORY_JSON_FILE_PATH);
    auto webSocketServer = new CPS::WebSocketServer(webSocketPort);

    // QObject::connect(rfidAuthenticator, &CPS::RfidAuthenticator::authenticated, rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::addItem);
    // QObject::connect(rfidAuthenticator, &CPS::RfidAuthenticator::authenticated, webSocketServer, &CPS::WebSocketServer::sendAuthenticatedUser);
    // QObject::connect(rfidAuthenticator, &CPS::RfidAuthenticator::unauthenticated, webSocketServer, &CPS::WebSocketServer::sendUnauthenticatedUser);

    QObject::connect(webSocketServer, &CPS::WebSocketServer::clientAuthenticationRequested, monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::authenticate);
    QObject::connect(monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::authenticated, webSocketServer, &CPS::WebSocketServer::authenticated);
    QObject::connect(monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::unauthenticated, webSocketServer, &CPS::WebSocketServer::unauthenticated);

    QObject::connect(webSocketServer, &CPS::WebSocketServer::historyRequested, rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::historyRequested);
    QObject::connect(rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::historyReady, webSocketServer, &CPS::WebSocketServer::sendHistory);

    QObject::connect(&a, &QCoreApplication::aboutToQuit, monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::deleteLater);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::deleteLater);
    // QObject::connect(httpServer, &CPS::HttpServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
