#ifndef ACCELERATION_H
#define ACCELERATION_H

#include <QObject>

struct Acceleration {
    Acceleration() : x(0), y(0), z(0) {}
    Acceleration(qreal x, qreal y, qreal z) : x(x), y(y), z(z) {}
    qreal x;
    qreal y;
    qreal z;
};

#endif // ACCELERATION_H
