#include <QCoreApplication>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QLoggingCategory>

#include "httpserver.h"
#include "monitoringsystemauthenticator.h"
#include "rfidauthenticationhistory.h"
#include "rfidauthenticator.h"
#include "websocketserver.h"

const QString MONITORING_SYSTEM_USERS_JSON_FILE_PATH = "data/monitoring_system_users.json";
const QString RFID_AUTHENTICATION_HISTORY_JSON_FILE_PATH = "data/monitoring_system_history.json";
const QString RFID_TAGS_JSON_FILE_PATH = "data/rfid_tags.json";

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    qSetMessagePattern("%{function} -> %{time} [%{type}] %{appname}: %{message}");
    QLoggingCategory::setFilterRules("*.debug=false");

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption webSocketPortOption("websocket-port", "Port for WebSocket server to listen on", "websocket-port", "12345");
    parser.addOption(webSocketPortOption);
    QCommandLineOption httpPortOption("http_port", "Port for Http server to listen on", "http_port", "54321");
    parser.addOption(httpPortOption);
    parser.process(a);

    int webSocketPort = parser.value(webSocketPortOption).toInt();
    int httpPort = parser.value(httpPortOption).toInt();

    qDebug() << "Starting CPS Server";

    auto monitoringSystemAuthenticator = new CPS::MonitoringSystemAuthenticator(MONITORING_SYSTEM_USERS_JSON_FILE_PATH);
    auto rfidAuthenticationHistory = new CPS::RfidAuthenticationHistory(RFID_AUTHENTICATION_HISTORY_JSON_FILE_PATH);
    auto rfidAuthenticator = new CPS::RfidAuthenticator(RFID_TAGS_JSON_FILE_PATH);

    auto webSocketServer = new CPS::WebSocketServer(webSocketPort);
    auto httpServer = new CPS::HttpServer(httpPort, rfidAuthenticator);

    QObject::connect(rfidAuthenticator, &CPS::RfidAuthenticator::authenticated, rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::addItem);
    QObject::connect(rfidAuthenticator, &CPS::RfidAuthenticator::authenticated, webSocketServer, &CPS::WebSocketServer::sendAuthenticatedUser);
    QObject::connect(rfidAuthenticator, &CPS::RfidAuthenticator::unauthenticated, webSocketServer, &CPS::WebSocketServer::sendUnauthenticatedUser);

    QObject::connect(webSocketServer, &CPS::WebSocketServer::clientAuthenticationRequested, monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::authenticate);
    QObject::connect(monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::authenticated, webSocketServer, &CPS::WebSocketServer::authenticated);
    QObject::connect(monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::unauthenticated, webSocketServer, &CPS::WebSocketServer::unauthenticated);

    QObject::connect(webSocketServer, &CPS::WebSocketServer::historyRequested, rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::historyRequested);
    QObject::connect(rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::historyReady, webSocketServer, &CPS::WebSocketServer::sendHistory);

    QObject::connect(&a, &QCoreApplication::aboutToQuit, monitoringSystemAuthenticator, &CPS::MonitoringSystemAuthenticator::deleteLater);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, rfidAuthenticationHistory, &CPS::RfidAuthenticationHistory::deleteLater);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, webSocketServer, &CPS::WebSocketServer::deleteLater);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]() {
        qDebug() << "CPS Server stopped";
    });

    return a.exec();
}
