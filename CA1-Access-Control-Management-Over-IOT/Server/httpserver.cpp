#include "httpserver.h"

#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace CPS {

HttpServer::HttpServer(int port, RfidAuthenticator* rfidAuthenticator)
    : port_{port}, server_(new QHttpServer()), rfidAuthenticator_(rfidAuthenticator) {
    server_->route("/authentication", [this](const QHttpServerRequest& request) {
        return this->authenticationHandler(request);
    });

    start();
}

void HttpServer::start() {
    if (!server_->listen(QHostAddress::Any, port_)) {
        qFatal("Failed to listen on port %d", port_);
        return;
    }
    qInfo() << "Listening on port" << port_;
}

void HttpServer::stop() {
    server_->disconnect();
}

QHttpServerResponse HttpServer::authenticationHandler(const QHttpServerRequest& request) {
    qDebug() << "Received request:" << request.url().toString();

    QByteArray jsonData = request.body();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (!jsonDoc.isObject()) {
        return QHttpServerResponse(QByteArray("Error: JSON is not an object"), QHttpServerResponse::StatusCode::BadRequest);
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (!jsonObj.contains("rfid") || !jsonObj["rfid"].isString()) {
        return QHttpServerResponse(QByteArray("Error: JSON object does not contain a 'rfid' string"), QHttpServerResponse::StatusCode::BadRequest);
    }

    QString rfid = jsonObj["rfid"].toString();
    RfidAuthenticationItem rfidAuthenticationItem(rfid);
    QJsonDocument responseJsonDoc(rfidAuthenticationItem.toJsonObject());
    QByteArray responseJsonData = responseJsonDoc.toJson();
    QHttpServerResponse::StatusCode statusCode = rfidAuthenticator_->authenticate(rfidAuthenticationItem)
                                                     ? QHttpServerResponse::StatusCode::Ok
                                                     : QHttpServerResponse::StatusCode::Unauthorized;

    return QHttpServerResponse(responseJsonData, statusCode);
}

HttpServer::~HttpServer() {
    server_->deleteLater();
}

} // namespace CPS
