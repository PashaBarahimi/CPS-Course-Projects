#ifndef RFIDAUTHENTICATIONHISTORY_H
#define RFIDAUTHENTICATIONHISTORY_H

#include <QJsonArray>
#include <QObject>

#include "rfidauthenticationitem.h"

namespace CPS {

class RfidAuthenticationHistory : public QObject {
    Q_OBJECT
public:
    RfidAuthenticationHistory(const QString& historyFilePath);
    ~RfidAuthenticationHistory();

Q_SIGNALS:
    void historyReady(const QJsonArray& history);

public Q_SLOTS:
    void addItem(const RfidAuthenticationItem& item);
    void historyRequested();

private:
    void loadHistory();
    void saveHistory();

private:
    QString historyFilePath_;
    QList<RfidAuthenticationItem> history_;
};

} // namespace CPS

#endif // RFIDAUTHENTICATIONHISTORY_H
