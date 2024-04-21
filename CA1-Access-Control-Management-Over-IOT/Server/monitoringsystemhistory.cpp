#include "monitoringsystemhistory.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "utils.h"

namespace CPS {

MonitoringSystemHistory::MonitoringSystemHistory(const QString& historyFilePath)
{
    historyFilePath_ = historyFilePath;
    loadHistory();
}

MonitoringSystemHistory::~MonitoringSystemHistory()
{
    saveHistory();
}

void MonitoringSystemHistory::addItem(const MonitoringSystemAuthenticationItem &item)
{
    history_.append(item);
}

QJsonArray MonitoringSystemHistory::getHistory() const
{
    QJsonArray array;
    for (const auto& item : history_)
    {
        array.append(item.toJsonObject());
    }

    return array;
}

void MonitoringSystemHistory::loadHistory()
{
    history_.clear();

    QByteArray data = Utils::readFile(historyFilePath_);
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    for (const auto& item : array)
    {
        MonitoringSystemAuthenticationItem authItem(item.toObject());
        history_.append(authItem);
    }
}

void MonitoringSystemHistory::saveHistory()
{
    QJsonArray array;
    for (const auto& item : history_)
    {
        array.append(item.toJsonObject());
    }

    QJsonDocument doc(array);
    QByteArray data = doc.toJson();
    Utils::writeFile(historyFilePath_, data);
}

} // namespace CPS
