#ifndef MONITORINGSYSTEMHISTORY_H
#define MONITORINGSYSTEMHISTORY_H

#include <QJsonArray>

#include "monitoringsystemauthenticationitem.h"

namespace CPS {

class MonitoringSystemHistory
{
public:
    MonitoringSystemHistory(const QString& historyFilePath);
    ~MonitoringSystemHistory();
    void addItem(const MonitoringSystemAuthenticationItem &item);
    QJsonArray getHistory() const;

private:
    void loadHistory();
    void saveHistory();

private:
    QString historyFilePath_;
    QList<MonitoringSystemAuthenticationItem> history_;
};

} // namespace CPS

#endif // MONITORINGSYSTEMHISTORY_H
