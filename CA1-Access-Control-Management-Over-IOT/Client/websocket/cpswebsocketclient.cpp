#include "cpswebsocketclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

#include "../users/cpsuser.h"
#include "cpswebsocketrequest.h"
#include "cpswebsocketresponse.h"

namespace CPS {

WebSocketClient::WebSocketClient(QObject* parent)
    : QObject(parent),
      webSocket_(new QWebSocket),
      user_(nullptr) {
    QObject::connect(webSocket_, &QWebSocket::connected, this, &WebSocketClient::wsConnected);
    QObject::connect(webSocket_, &QWebSocket::disconnected, this, &WebSocketClient::wsDisconnected);
    QObject::connect(webSocket_, &QWebSocket::errorOccurred, this, &WebSocketClient::wsError);
    QObject::connect(webSocket_, &QWebSocket::textMessageReceived, this, &WebSocketClient::processTextMessage);
}

WebSocketClient::~WebSocketClient() {
    delete webSocket_;
    delete user_;
}

void WebSocketClient::connectToServer(const QString& address, const QString& username, const QString& password) {
    qInfo() << "Connecting to:" << address << "...";
    Q_EMIT connectionChanged(false);
    user_ = new User(username, password);
    webSocket_->open(QUrl(address));
}

void WebSocketClient::wsConnected() {
    qInfo() << "Connected to server";
}

void WebSocketClient::wsDisconnected() {
    qInfo() << "Websocket disconnected";
}

void WebSocketClient::wsError(QAbstractSocket::SocketError err) {
    qWarning() << "Websocket error:" << err;
    closeConnection();
}

void WebSocketClient::processTextMessage(const QString& message) {
    qDebug() << "Received message from server:" << message;
    QJsonObject json = QJsonDocument::fromJson(message.toUtf8()).object();
    WebSocketResponse response(json);

    if (response.status() == WebSocketResponse::Status::Unknown ||
        response.type() == WebSocketResponse::Type::Unknown) {
        qInfo() << "Invalid response";
        return;
    }

    switch (response.type()) {
    case WebSocketResponse::Type::Unknown:
        break;
    case WebSocketResponse::Type::Authenticate:
        break;
    case WebSocketResponse::Type::HistoryResponse:
        break;
    case WebSocketResponse::Type::Rfid:
        break;
    }
}

void WebSocketClient::closeConnection() {
    webSocket_->close();
    delete user_;
    user_ = nullptr;
    Q_EMIT connectionChanged(true);
}

void WebSocketClient::sendRequest(const WebSocketRequest& req) {
    webSocket_->sendTextMessage(req.toJsonDocument().toJson(QJsonDocument::Compact));
}

} // namespace CPS
