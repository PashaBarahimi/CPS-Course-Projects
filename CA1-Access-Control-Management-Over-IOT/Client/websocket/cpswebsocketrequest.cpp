#include "cpswebsocketrequest.h"

namespace CPS {

WebSocketRequest::WebSocketRequest(Type type, const QJsonObject& data)
    : type_(type),
      data_(data) {}

QJsonDocument WebSocketRequest::toJsonDocument() const {
    QJsonObject json;
    json["type"] = typeToString(type_);
    json["data"] = data_;
    return QJsonDocument(json);
}

QString WebSocketRequest::typeToString(Type type) {
    switch (type) {
    case Type::Authenticate:
        return "authenticate";
    case Type::HistoryRequest:
        return "history_request";
    }
}

} // namespace CPS
