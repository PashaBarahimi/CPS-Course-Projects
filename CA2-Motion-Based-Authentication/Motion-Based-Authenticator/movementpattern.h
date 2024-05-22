#ifndef MOVEMENTPATTERN_H
#define MOVEMENTPATTERN_H

#include <QObject>
#include <QVector>
#include "movement.h"

class MovementPattern {
public:
    void addMovement(Movement* movement) {
        pattern_.append(movement);
    }

    const QVector<Movement*>& getPattern() const {
        return pattern_;
    }

    bool matches(const MovementPattern& other) const;

private:
    QVector<Movement*> pattern_;
};

#endif // MOVEMENTPATTERN_H
