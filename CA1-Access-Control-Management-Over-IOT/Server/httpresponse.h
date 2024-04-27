#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QJsonDocument>

namespace CPS {
class HttpResponse : public QObject {
public:
    HttpResponse(bool authenticated, const QJsonDocument& data = QJsonDocument());
    QJsonDocument toJsonDocument() const;

private:
    bool authenticated_;
    QJsonDocument m_data;
};
} // namespace CPS

#endif // HTTPRESPONSE_H
