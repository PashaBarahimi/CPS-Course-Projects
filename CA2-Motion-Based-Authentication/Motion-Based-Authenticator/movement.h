#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>
#include <QPointF>
#include <QtQml/qqmlregistration.h>

#include "direction.h"
#include "angle.h"

class Movement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF start READ start WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QPointF end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(Direction::Type direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(Angle::Type angle READ angle WRITE setAngle NOTIFY angleChanged)
    QML_ELEMENT

public:
    explicit Movement(QObject *parent = nullptr);
    Movement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle, QObject *parent = nullptr);

    QPointF start() const;
    QPointF end() const;
    Direction::Type direction() const;
    Angle::Type angle() const;

    void setStart(QPointF start);
    void setEnd(QPointF end);
    void setDirection(Direction::Type direction);
    void setAngle(Angle::Type angle);

    Q_INVOKABLE QString toStr(int current, int total) const;
    QJsonObject toJson() const;

Q_SIGNALS:
    void startChanged();
    void endChanged();
    void directionChanged();
    void angleChanged();

private:
    QPointF start_;
    QPointF end_;
    Direction::Type direction_;
    Angle::Type angle_;
};
Q_DECLARE_METATYPE(Movement)

#endif // MOVEMENT_H
