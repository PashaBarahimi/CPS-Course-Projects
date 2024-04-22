#ifndef WEBSOCKETRESPONSE_H
#define WEBSOCKETRESPONSE_H

#include <QJsonDocument>

namespace CPS {

class WebSocketResponse {
public:
    enum class Status {
        Ok = 200,
        BadRequest = 400,
        Unauthorized = 401
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
