#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>

#include "QtWebSockets/qwebsocket.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "rfidauthenticationitem.h"
#include "websocketresponse.h"

namespace CPS {

class WebSocketServer : public QObject {
    Q_OBJECT
public:
    WebSocketServer(int port, QObject* parent = nullptr);

Q_SIGNALS:
    void closed();
    void clientAuthenticationRequested(const QString& username, const QString& encodedPassword);
    void historyRequested();

public Q_SLOTS:
    void onNewConnection();
    void processTextMessage(const QString& message);
    void socketDisconnected();

    void authenticated();
    void unauthenticated();
    void sendAuthenticatedUser(const RfidAuthenticationItem& item);
    void sendUnauthenticatedUser(const RfidAuthenticationItem& item);
    void sendHistory(const QJsonArray& history);

private Q_SLOTS:
    void sendTextMessage(const WebSocketResponse& response);

private:
    int port_;
    bool authenticated_;
    QWebSocketServer* server_;
    QWebSocket* client_;
};

} // namespace CPS

#endif // WEBSOCKETSERVER_H
