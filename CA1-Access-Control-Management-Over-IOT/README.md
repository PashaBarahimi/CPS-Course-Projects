# CA1 - Access Control Management Over IOT

- [CA1 - Access Control Management Over IOT](#ca1---access-control-management-over-iot)
  - [Introduction](#introduction)
  - [Description](#description)
    - [Embedded](#embedded)
    - [Proteus](#proteus)
    - [Server](#server)
      - [HTTP Server](#http-server)
      - [WebSocket Server](#websocket-server)
    - [Client](#client)
  - [Explanation](#explanation)
    - [Embedded](#embedded-1)
    - [Proteus](#proteus-1)
    - [Server](#server-1)
      - [HTTP Server](#http-server-1)
      - [WebSocket Server](#websocket-server-1)
        - [Monitoring System User](#monitoring-system-user)
        - [Monitoring System Authenticator](#monitoring-system-authenticator)
        - [RFID Authentication Item](#rfid-authentication-item)
        - [RFID Authentication History](#rfid-authentication-history)
        - [WebSocket Request](#websocket-request)
        - [WebSocket Response](#websocket-response)
        - [WebSocket Server](#websocket-server-2)
    - [Client](#client-1)
  - [How to Run](#how-to-run)
  - [RFID](#rfid)

## Introduction

## Description

### Embedded

### Proteus

### Server

#### HTTP Server

#### WebSocket Server

The WebSocket server is responsible for handling the communication between the Monitoring System client and the server. The server is implemented using the `QtWebSockets` library and it listens on port `12345` by default. However, this can be changed by providing the `--websocket-port` argument when running the server. It is responsible for handling the following messages:

- `authenticate`: This message is sent by the client when it wants to authenticate a user. The server will then check if the user is allowed to access the system and respond with a message indicating if the user is allowed or not. The message contains the username and password (base64-encoded) of the user that is trying to authenticate. The server will then respond with a json message with status set to either 200 (OK) or 401 (Unauthorized). The messages format are as follows:

  ```json
  // client -> server
  {
      "type": "authenticate",
      "data": {
          "username": "admin",
          "password": "MTIzNA=="
      }
  }
  ```

  ```json
  // server -> client
  {
      "status": 200 | 401,
      "data": {}
  }
  ```

- `history_request`: This message is sent by the client to retrieve the RFID access history. The server will then respond with a message containing the access history. If the user is not authenticated, the server will respond with a status of 401 (Unauthorized). The messages format are as follows:

  ```json
  // client -> server
  {
      "type": "history_request",
      "data": {}
  }
  ```

  ```json
  // server -> client
  {
      "status": 200 | 401,
      "data": [
          {
              "username": "1234567890",
              "time": "10:10",
              "date": "10/28/2021"
          }
      ]
  }
  ```

- `access_request`: This message is sent by the server to the client whenever a user tries to authenticate using the RFID reader. The server will then send a message to the client containing the username of the user that is trying to authenticate. The client will then display a message to the user indicating that the user is trying to authenticate. The status of the message will either be 200 (OK) or 403 (Forbidden). The messages format are as follows:

  ```json
  // server -> client
  {
      "status": 200 | 403,
      "data": {
          "username": "1234567890",
          "time": "10:10",
          "date": "10/28/2021"
      }
  }
  ```

The access request history is stored in `data/monitoring_system_history.json` and the client authentication information is stored in `data/monitoring_system_users.json`.

### Client

## Explanation

### Embedded

### Proteus

### Server

#### HTTP Server

#### WebSocket Server

##### Monitoring System User

This class has the header file `monitoringsystemuser.h` and the implementation file `monitoringsystemuser.cpp`. It is the data structure that holds the information of the user that is trying to authenticate. The header file contains the following:

```cpp
#ifndef MONITORINGSYSTEMUSER_H
#define MONITORINGSYSTEMUSER_H

#include <QJsonObject>
#include <QString>

namespace CPS {

class MonitoringSystemUser {
public:
    MonitoringSystemUser(const QString& username, const QString& hashedPassword);
    MonitoringSystemUser(const QJsonObject& jsonObject);
    QString username() const;
    QString hashedPassword() const;
    QJsonObject toJsonObject() const;

private:
    QString username_;
    QString hashedPassword_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMUSER_H
```

It can be constructed using the username and hashed password of the user. It also has a constructor that takes a `QJsonObject` as an argument and constructs the object from the json object. It also has a `toJsonObject` method that converts the object to a `QJsonObject`. The implementation file contains the following:

```cpp
#include "monitoringsystemuser.h"

namespace CPS {

MonitoringSystemUser::MonitoringSystemUser(const QString& username, const QString& hashedPassword)
    : username_(username), hashedPassword_(hashedPassword) {}

MonitoringSystemUser::MonitoringSystemUser(const QJsonObject& jsonObject) {
    username_ = jsonObject["username"].toString();
    hashedPassword_ = jsonObject["hashedPassword"].toString();
}

QString MonitoringSystemUser::username() const { return username_; }

QString MonitoringSystemUser::hashedPassword() const { return hashedPassword_; }

QJsonObject MonitoringSystemUser::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject["username"] = username_;
    jsonObject["hashedPassword"] = hashedPassword_;
    return jsonObject;
}

} // namespace CPS
```

##### Monitoring System Authenticator

This class has the header file `monitoringsystemauthenticator.h` and the implementation file `monitoringsystemauthenticator.cpp`. It is responsible for authenticating the user that is trying to authenticate. The header file contains the following:

```cpp
#ifndef MONITORINGSYSTEMAUTHENTICATOR_H
#define MONITORINGSYSTEMAUTHENTICATOR_H

#include <QJsonArray>
#include <QList>
#include <QObject>
#include <QString>

#include "monitoringsystemuser.h"

namespace CPS {

class MonitoringSystemAuthenticator : public QObject {
    Q_OBJECT
public:
    MonitoringSystemAuthenticator(const QString& jsonFilePath);
    ~MonitoringSystemAuthenticator();
    void addUser(const QString& username, const QString& password);

Q_SIGNALS:
    void authenticated();
    void unauthenticated();

public Q_SLOTS:
    void authenticate(const QString& username, const QString& password);

private:
    void loadUsers();
    void saveUsers();
    QString decodePassword(const QString& encodedPassword);
    QString hashPassword(const QString& password);

private:
    QString jsonFilePath_;
    QList<MonitoringSystemUser> users_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMAUTHENTICATOR_H
```

It can be constructed using the path to the json file that contains the user information. It has a method `addUser` that adds a user to the list of users. It also has a signal `authenticated` that is emitted when the user is authenticated and a signal `unauthenticated` that is emitted when the user is not authenticated. It also has a slot `authenticate` that is called when the user tries to authenticate. The implementation file contains the following:

```cpp
#include "monitoringsystemauthenticator.h"

#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils.h"

namespace CPS {

MonitoringSystemAuthenticator::MonitoringSystemAuthenticator(const QString& jsonFilePath)
    : jsonFilePath_(jsonFilePath) {
    loadUsers();
}

MonitoringSystemAuthenticator::~MonitoringSystemAuthenticator() { saveUsers(); }

void MonitoringSystemAuthenticator::authenticate(const QString& username, const QString& password) {
    QString decodedPassword = decodePassword(password);
    QString hashedPassword = hashPassword(decodedPassword);
    for (const MonitoringSystemUser& user : users_) {
        if (user.username() == username &&
            user.hashedPassword() == hashedPassword) {
            qDebug() << "User" << username << "authenticated";
            Q_EMIT authenticated();
            return;
        }
    }
    qDebug() << "User" << username << "not authenticated";
    Q_EMIT unauthenticated();
}

void MonitoringSystemAuthenticator::addUser(const QString& username, const QString& password) {
    QString hashedPassword = hashPassword(password);
    MonitoringSystemUser user(username, hashedPassword);
    users_ << user;
    qDebug() << "User" << username << "added";
}

void MonitoringSystemAuthenticator::loadUsers() {
    qDebug() << "Loading users from" << jsonFilePath_;
    QByteArray jsonFile = Utils::readFile(jsonFilePath_);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonFile);
    QJsonArray jsonArray = jsonDocument.array();

    for (const QJsonValue& jsonValue : jsonArray) {
        QJsonObject jsonObject = jsonValue.toObject();
        MonitoringSystemUser user(jsonObject);
        users_ << user;
        qDebug() << "User" << user.username() << "loaded";
    }
}

void MonitoringSystemAuthenticator::saveUsers() {
    qDebug() << "Saving users to" << jsonFilePath_;
    QJsonArray jsonArray;
    for (const MonitoringSystemUser& user : users_) {
        jsonArray << user.toJsonObject();
    }

    QJsonDocument jsonDocument(jsonArray);
    QByteArray jsonFile = jsonDocument.toJson();
    Utils::writeFile(jsonFilePath_, jsonFile);
}

QString MonitoringSystemAuthenticator::decodePassword(const QString& encodedPassword) {
    QByteArray decodedPassword = QByteArray::fromBase64(encodedPassword.toUtf8());
    return QString::fromUtf8(decodedPassword);
}

QString MonitoringSystemAuthenticator::hashPassword(const QString& password) {
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromUtf8(hashedPassword.toHex());
}

} // namespace CPS
```

The `loadUser` method loads the users from the json file and the `saveUsers` method saves the users to the json file. The `decodePassword` method decodes the base64-encoded password and the `hashPassword` method hashes the password using SHA-256 algorithm.

##### RFID Authentication Item

This class has the header file `rfidauthenticationitem.h` and the implementation file `rfidauthenticationitem.cpp`. It is the data structure that holds the information of the RFID authentication item. The header file contains the following:

```cpp
#ifndef RFIDAUTHENTICATIONITEM_H
#define RFIDAUTHENTICATIONITEM_H

#include <QJsonObject>
#include <QString>

namespace CPS {

class RfidAuthenticationItem {
public:
    RfidAuthenticationItem(const QString& username, const QString& time, const QString& date);
    RfidAuthenticationItem(const QJsonObject& jsonObject);
    RfidAuthenticationItem(const QString& username);
    QJsonObject toJsonObject() const;
    QString username() const;

private:
    QString username_;
    QString time_;
    QString date_;
};

} // namespace CPS

#endif // RFIDAUTHENTICATIONITEM_H
```

It can be constructed using the username, time, and date of the RFID authentication item. It also has a constructor that takes a `QJsonObject` as an argument and constructs the object from the json object. This class has a third constructor that takes only the username of the RFID authentication item. In this case, the time and date are set to the current time and date. It also has a `toJsonObject` method that converts the object to a `QJsonObject`. The implementation file contains the following:

```cpp
#include "rfidauthenticationitem.h"

#include <QDateTime>

namespace CPS {

RfidAuthenticationItem::RfidAuthenticationItem(const QString& username, const QString& time, const QString& date)
    : username_(username), time_(time), date_(date) {}

RfidAuthenticationItem::RfidAuthenticationItem(const QJsonObject& jsonObject)
    : username_(jsonObject["username"].toString()),
      time_(jsonObject["time"].toString()),
      date_(jsonObject["date"].toString()) {}

RfidAuthenticationItem::RfidAuthenticationItem(const QString& username)
    : username_(username),
      time_(QDateTime::currentDateTime().toString("hh:mm")),
      date_(QDateTime::currentDateTime().toString("MM/dd/yyyy")) {}

QJsonObject RfidAuthenticationItem::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject["username"] = username_;
    jsonObject["time"] = time_;
    jsonObject["date"] = date_;
    return jsonObject;
}

QString RfidAuthenticationItem::username() const { return username_; }

} // namespace CPS
```

##### RFID Authentication History

This class has the header file `rfidauthenticationhistory.h` and the implementation file `rfidauthenticationhistory.cpp`. It is responsible for storing the RFID authentication history. The header file contains the following:

```cpp
#ifndef RFIDAUTHENTICATIONHISTORY_H
#define RFIDAUTHENTICATIONHISTORY_H

#include <QJsonArray>
#include <QObject>

#include "rfidauthenticationitem.h"

namespace CPS {

class RfidAuthenticationHistory : public QObject {
    Q_OBJECT
public:
    RfidAuthenticationHistory(const QString& historyFilePath);
    ~RfidAuthenticationHistory();

Q_SIGNALS:
    void historyReady(const QJsonArray& history);

public Q_SLOTS:
    void addItem(const RfidAuthenticationItem& item);
    void historyRequested();

private:
    void loadHistory();
    void saveHistory();

private:
    QString historyFilePath_;
    QList<RfidAuthenticationItem> history_;
};

} // namespace CPS

#endif // RFIDAUTHENTICATIONHISTORY_H
```

It can be constructed using the path to the json file that contains the RFID authentication history. It has a signal `historyReady` that is emitted when the history is ready. It also has a slot `addItem` that adds an item to the history and a slot `historyRequested` that is called when the history is requested. The implementation file contains the following:

```cpp
#include "rfidauthenticationhistory.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils.h"

namespace CPS {

RfidAuthenticationHistory::RfidAuthenticationHistory(const QString& historyFilePath) {
    historyFilePath_ = historyFilePath;
    loadHistory();
}

RfidAuthenticationHistory::~RfidAuthenticationHistory() { saveHistory(); }

void RfidAuthenticationHistory::addItem(const RfidAuthenticationItem& item) {
    history_ << item;
}

void RfidAuthenticationHistory::historyRequested() {
    QJsonArray array;
    for (const auto& item : history_) {
        array << item.toJsonObject();
    }

    Q_EMIT historyReady(array);
}

void RfidAuthenticationHistory::loadHistory() {
    history_.clear();
    qDebug() << "Loading history from" << historyFilePath_;

    QByteArray data = Utils::readFile(historyFilePath_);
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (const auto& item : array) {
        RfidAuthenticationItem authItem(item.toObject());
        history_ << authItem;
    }
}

void RfidAuthenticationHistory::saveHistory() {
    qDebug() << "Saving history to" << historyFilePath_;
    QJsonArray array;
    for (const auto& item : history_) {
        array << item.toJsonObject();
    }

    QJsonDocument doc(array);
    QByteArray data = doc.toJson();
    Utils::writeFile(historyFilePath_, data);
}

} // namespace CPS
```

The `loadHistory` method loads the history from the json file and the `saveHistory` method saves the history to the json file.

##### WebSocket Request

This class has the header file `websocketrequest.h` and the implementation file `websocketrequest.cpp`. It is responsible for handling the WebSocket requests. The header file contains the following:

```cpp
#ifndef WEBSOCKETREQUEST_H
#define WEBSOCKETREQUEST_H

#include <QJsonObject>

namespace CPS {

class WebSocketRequest {
public:
    enum class Type {
        Unknown,
        Authenticate,
        HistoryRequest
    };

    WebSocketRequest(const QJsonObject& json);
    Type type() const;
    QJsonObject data() const;

private:
    Type type_;
    QJsonObject data_;
};

} // namespace CPS

#endif // WEBSOCKETREQUEST_H
```

It can be constructed using a `QJsonObject` that contains the WebSocket request. It has a method `type` that returns the type of the request and a method `data` that returns the data of the request. The implementation file contains the following:

```cpp
#include "websocketrequest.h"

namespace CPS {

WebSocketRequest::WebSocketRequest(const QJsonObject& json) {
    if (json.contains("type") && json["type"].isString()) {
        QString type = json["type"].toString();
        if (type == "authenticate") {
            type_ = Type::Authenticate;
        }
        else if (type == "history_request") {
            type_ = Type::HistoryRequest;
        }
        else {
            type_ = Type::Unknown;
        }
    }

    if (json.contains("data") && json["data"].isObject()) {
        data_ = json["data"].toObject();
    }
}

WebSocketRequest::Type WebSocketRequest::type() const { return type_; }

QJsonObject WebSocketRequest::data() const { return data_; }

} // namespace CPS
```

##### WebSocket Response

This class has the header file `websocketresponse.h` and the implementation file `websocketresponse.cpp`. It is responsible for handling the WebSocket responses. The header file contains the following:

```cpp
#ifndef WEBSOCKETRESPONSE_H
#define WEBSOCKETRESPONSE_H

#include <QJsonDocument>

namespace CPS {

class WebSocketResponse {
public:
    enum class Status {
        Ok = 200,
        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
    };

    WebSocketResponse(Status status, const QJsonDocument& data = QJsonDocument());
    QJsonDocument toJsonDocument() const;
    int status() const;

private:
    Status status_;
    QJsonDocument data_;
};

} // namespace CPS

#endif // WEBSOCKETRESPONSE_H
```

It can be constructed using the status of the response and the data of the response. It has a method `toJsonDocument` that converts the response to a `QJsonDocument` and a method `status` that returns the status of the response. The implementation file contains the following:

```cpp
#include "websocketresponse.h"

#include <QJsonArray>
#include <QJsonObject>

namespace CPS {

WebSocketResponse::WebSocketResponse(Status status, const QJsonDocument& data)
    : status_(status), data_(data) {}

QJsonDocument WebSocketResponse::toJsonDocument() const {
    QJsonObject json;
    json["status"] = static_cast<int>(status_);
    if (data_.isArray()) {
        json["data"] = data_.array();
    }
    else {
        json["data"] = data_.object();
    }
    return QJsonDocument(json);
}

int WebSocketResponse::status() const { return static_cast<int>(status_); }

} // namespace CPS
```

##### WebSocket Server

This class has the header file `websocketserver.h` and the implementation file `websocketserver.cpp`. It is responsible for handling the WebSocket communication between the server and the client. The header file contains the following:

```cpp
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
    ~WebSocketServer();

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
```

It can be constructed using the port that the server will listen on. It has a signal `closed` that is emitted when the server is closed, a signal `clientAuthenticationRequested` that is emitted when the client requests authentication, and a signal `historyRequested` that is emitted when the client requests the history. It also has a slot `onNewConnection` that is called when a new connection is made, a slot `processTextMessage` that is called when a text message is received, and a slot `socketDisconnected` that is called when the socket is disconnected. It also has a slot `authenticated` that is called when the user is authenticated, a slot `unauthenticated` that is called when the user is not authenticated, a slot `sendAuthenticatedUser` that is called when the user is authenticated using the RFID reader, a slot `sendUnauthenticatedUser` that is called when the user is not authenticated using the RFID reader, and a slot `sendHistory` that is called when the history is ready. The implementation file contains the following:

```cpp
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

WebSocketServer::~WebSocketServer() {
    server_->close();
    qInfo() << "Server closed";
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
            Q_EMIT clientAuthenticationRequested(username, password);
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
        Q_EMIT historyRequested();
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
```

Whenever a new connection is made, the `onNewConnection` slot is called which sets the `authenticated` flag to false and connects the `textMessageReceived` signal to the `processTextMessage` slot and the `disconnected` signal to the `socketDisconnected` slot. The `processTextMessage` slot processes the text message received from the client. If the message is an `authenticate` message, it emits the `clientAuthenticationRequested` signal with the username and password of the user. If the message is a `history_request` message, it checks if the user is authenticated and emits the `historyRequested` signal. If the message is neither an `authenticate` message nor a `history_request` message, it sends a `BadRequest` response. The `authenticated` slot sets the `authenticated` flag to true and sends an `Ok` response. The `unauthenticated` slot sets the `authenticated` flag to false and sends an `Unauthorized` response. The `sendAuthenticatedUser` slot sends an `Ok` response with the authenticated user. The `sendUnauthenticatedUser` slot sends a `Forbidden` response with the unauthenticated user. The `sendHistory` slot sends an `Ok` response with the history. The `sendTextMessage` slot sends the response to the client.

### Client

## How to Run

## RFID
