#ifndef CPSWEBSOCKETREQUEST_H
#define CPSWEBSOCKETREQUEST_H

#include <QJsonDocument>
#include <QJsonObject>

namespace CPS {

class WebSocketRequest {
public:
    enum class Type {
        Authenticate,
        HistoryRequest
    };

    WebSocketRequest(Type type, const QJsonObject& data = QJsonObject());
    QJsonDocument toJsonDocument() const;

private:
    static QString typeToString(Type type);

private:
    Type type_;
    QJsonObject data_;
};

} // namespace CPS

#endif // CPSWEBSOCKETREQUEST_H
