#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>

class httprequest : public QObject {
    Q_OBJECT
public:
    explicit httprequest(QObject* parent = nullptr);

signals:
};

#endif // HTTPREQUEST_H
