#ifndef CPSWEBSOCKETCLIENT_H
#define CPSWEBSOCKETCLIENT_H

#include <QJsonArray>
#include <QObject>
#include <QString>
#include <QWebSocket>

namespace CPS {

class User;
class WebSocketRequest;

class WebSocketClient : public QObject {
    Q_OBJECT
public:
    explicit WebSocketClient(QObject* parent = nullptr);
    ~WebSocketClient();

Q_SIGNALS:
    void newUser(const QString& username, const QString& date, const QString& time, bool denied);
    void connectionChanged(bool enabled);

public Q_SLOTS:
    void connectToServer(const QString& address, const QString& username, const QString& password);

private Q_SLOTS:
    void wsConnected();
    void wsDisconnected();
    void wsError(QAbstractSocket::SocketError err);
    void processTextMessage(const QString& message);

private:
    void closeConnection();
    void sendAuthenticationRequest();
    void sendRequest(const WebSocketRequest& req);

private:
    QWebSocket* webSocket_;
    User* user_;
};

} // namespace CPS

#endif // CPSWEBSOCKETCLIENT_H
