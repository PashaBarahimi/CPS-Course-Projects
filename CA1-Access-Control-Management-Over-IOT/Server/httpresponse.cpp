#include "httpresponse.h"

#include <QJsonArray>
#include <QJsonObject>

namespace CPS {

HttpResponse::HttpResponse(bool authenticated, const QJsonDocument& data)
    : authenticated_(authenticated), m_data{data} {}

} // namespace CPS
