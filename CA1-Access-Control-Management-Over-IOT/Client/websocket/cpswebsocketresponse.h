#ifndef CPSWEBSOCKETRESPONSE_H
#define CPSWEBSOCKETRESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>

namespace CPS {

class WebSocketResponse {
public:
    enum class Type {
        Unknown,
        Authenticate,
        HistoryResponse,
        Rfid,
    };

    enum class Status {
        Unknown = 0,
        Ok = 200,
        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
    };

    WebSocketResponse(const QJsonObject& json);
    Type type() const;
    Status status() const;
    QJsonDocument data() const;

private:
    static Status statusFromInt(int status);

private:
    Status status_;
    Type type_;
    QJsonDocument data_;
};

} // namespace CPS

#endif // CPSWEBSOCKETRESPONSE_H
