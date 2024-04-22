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
