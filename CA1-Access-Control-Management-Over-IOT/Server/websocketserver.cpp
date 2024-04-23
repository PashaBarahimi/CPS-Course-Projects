#include "websocketserver.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "websocketrequest.h"

namespace CPS {

WebSocketServer::WebSocketServer(int port, QObject* parent)
    : QObject(parent), port_(port), authenticated_(false), server_(new QWebSocketServer("CPS WebSocket Server", QWebSocketServer::NonSecureMode, this)), client_(nullptr) {
    if (!server_->listen(QHostAddress::Any, port_)) {
        qFatal("Failed to listen on port %d", port_);
    }
    qInfo() << "Listening on port" << port_;

    connect(server_, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
}

void WebSocketServer::onNewConnection() {
    client_ = server_->nextPendingConnection();
    authenticated_ = false;
    connect(client_, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(client_, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);
    qInfo() << "Client connected";
}

void WebSocketServer::processTextMessage(const QString& message) {
    QJsonObject json = QJsonDocument::fromJson(message.toUtf8()).object();
    WebSocketRequest request(json);
    qInfo() << "Received message:" << json;

    if (request.type() == WebSocketRequest::Type::Authenticate) {
        if (request.data() == QJsonObject()) {
            qDebug() << "Empty data object";
            sendTextMessage(WebSocketResponse(WebSocketResponse::Status::BadRequest));
            return;
        }
        QJsonObject data = request.data();
        if (data.contains("username") && data.contains("password")) {
            QString username = data["username"].toString();
            QString password = data["password"].toString();
            emit clientAuthenticationRequested(username, password);
            qDebug() << "Authenticating user:" << username;
        }
        else {
            qDebug() << "Invalid data object";
            sendTextMessage(WebSocketResponse(WebSocketResponse::Status::BadRequest));
        }
    }

    else if (request.type() == WebSocketRequest::Type::HistoryRequest) {
        if (!authenticated_) {
            qDebug() << "Unauthorized";
            sendTextMessage(WebSocketResponse(WebSocketResponse::Status::Unauthorized));
            return;
        }
        emit historyRequested();
        qDebug() << "History requested";
    }

    else {
        sendTextMessage(WebSocketResponse(WebSocketResponse::Status::BadRequest));
    }
}

void WebSocketServer::socketDisconnected() {
    qInfo() << "Client disconnected";
    client_->deleteLater();
}

void WebSocketServer::authenticated() {
    authenticated_ = true;
    qDebug() << "Authenticated";
    sendTextMessage(WebSocketResponse(WebSocketResponse::Status::Ok));
}

void WebSocketServer::unauthenticated() {
    authenticated_ = false;
    qDebug() << "Authentication failed";
    sendTextMessage(WebSocketResponse(WebSocketResponse::Status::Unauthorized));
}

void WebSocketServer::sendAuthenticatedUser(const RfidAuthenticationItem& item) {
    WebSocketResponse response(WebSocketResponse::Status::Ok, QJsonDocument(item.toJsonObject()));
    qDebug() << "Sending authenticated user:" << item.username();
    sendTextMessage(response);
}

void WebSocketServer::sendUnauthenticatedUser(const RfidAuthenticationItem& item) {
    WebSocketResponse response(WebSocketResponse::Status::Forbidden, QJsonDocument(item.toJsonObject()));
    qDebug() << "Sending unauthenticated user:" << item.username();
    sendTextMessage(response);
}

void WebSocketServer::sendHistory(const QJsonArray& history) {
    WebSocketResponse response(WebSocketResponse::Status::Ok, QJsonDocument(history));
    qDebug() << "Sending history with" << history.size() << "item(s)";
    sendTextMessage(response);
}

void WebSocketServer::sendTextMessage(const WebSocketResponse& response) {
    if (client_) {
        qInfo() << "Sending message with status" << response.status();
        client_->sendTextMessage(response.toJsonDocument().toJson(QJsonDocument::Compact));
        return;
    }
    qWarning() << "No client connected";
}

} // namespace CPS
