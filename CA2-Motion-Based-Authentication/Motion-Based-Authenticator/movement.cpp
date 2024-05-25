#include "movement.h"

Movement::Movement(QObject* parent)
    : QObject(parent) {}

Movement::Movement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle, QObject* parent)
    : QObject(parent), start_(start), end_(end), direction_(direction), angle_(angle) {}

QPointF Movement::start() const {
    return start_;
}

QPointF Movement::end() const {
    return end_;
}

Direction::Type Movement::direction() const {
    return direction_;
}

Angle::Type Movement::angle() const {
    return angle_;
}

void Movement::setStart(QPointF start) {
    if (start_ != start) {
        start_ = start;
        Q_EMIT startChanged();
    }
}

void Movement::setEnd(QPointF end) {
    if (end_ != end) {
        end_ = end;
        Q_EMIT endChanged();
    }
}

void Movement::setDirection(Direction::Type direction) {
    if (direction_ != direction) {
        direction_ = direction;
        Q_EMIT directionChanged();
    }
}

void Movement::setAngle(Angle::Type angle) {
    if (angle_ != angle) {
        angle_ = angle;
        Q_EMIT angleChanged();
    }
}

QString Movement::toStr(int current, int total) const {
    QString result;
    result.append(QString("Movement %1 of %2\n").arg(current).arg(total));
    result.append(QString("\tStart:\n\t\tX: %1\n\t\tY: %2\n").arg(start_.x()).arg(start_.y()));
    result.append(QString("\tEnd:\n\t\tX: %1\n\t\tY: %2\n").arg(end_.x()).arg(end_.y()));
    result.append(QString("\tDirection: %1\n").arg(Direction::toString(direction_)));
    result.append(QString("\tAngle: %1\n").arg(Angle::toString(angle_)));
    return result;
}

QJsonObject Movement::toJson() const {
    QJsonObject json;
    json["start"] = QJsonObject{
        {"x", start_.x()},
        {"y", start_.y()}};
    json["end"] = QJsonObject{
        {"x", end_.x()},
        {"y", end_.y()}};
    json["direction"] = Direction::toString(direction_);
    json["angle"] = Angle::toString(angle_);
    return json;
}
