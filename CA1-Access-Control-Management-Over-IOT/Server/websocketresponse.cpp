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
