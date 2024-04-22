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
