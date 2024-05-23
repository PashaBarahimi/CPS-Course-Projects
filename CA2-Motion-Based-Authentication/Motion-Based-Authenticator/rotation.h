#ifndef ROTATION_H
#define ROTATION_H

#include <QObject>

class Rotation {
public:
    Rotation(qreal x = 0, qreal y = 0, qreal z = 0)
        : x(x), y(y), z(z) {}

    qreal x;
    qreal y;
    qreal z;
};

#endif // ROTATION_H
