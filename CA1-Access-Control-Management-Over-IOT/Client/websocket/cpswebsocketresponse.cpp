#include "cpswebsocketresponse.h"

#include <QJsonArray>

namespace CPS {

WebSocketResponse::WebSocketResponse(const QJsonObject& json) {
    if (json.contains("status")) {
        int status = json["status"].toInt();
        status_ = statusFromInt(status);
    }
    if (json.contains("data")) {
        if (json["data"].isObject()) {
            data_ = QJsonDocument(json["data"].toObject());
            if (data_.object().isEmpty()) {
                type_ = Type::Authenticate;
            }
            else {
                type_ = Type::Rfid;
            }
        }
        else if (json["data"].isArray()) {
            data_ = QJsonDocument(json["data"].toArray());
            type_ = Type::HistoryResponse;
        }
    }
    else {
        type_ = Type::Unknown;
    }
}

WebSocketResponse::Type WebSocketResponse::type() const { return type_; }
WebSocketResponse::Status WebSocketResponse::status() const { return status_; }
QJsonDocument WebSocketResponse::data() const { return data_; }

WebSocketResponse::Status WebSocketResponse::statusFromInt(int status) {
    switch (static_cast<Status>(status)) {
    case Status::Ok:
    case Status::BadRequest:
    case Status::Unauthorized:
    case Status::Forbidden:
        return static_cast<Status>(status);
    default:
        return Status::Unknown;
    }
}

} // namespace CPS
