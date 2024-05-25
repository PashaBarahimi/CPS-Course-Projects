#include "movementpattern.h"

#include <cmath>

bool MovementPattern::matches(const MovementPattern& other) const {
    if (pattern_.size() != other.pattern_.size()) return false;

    const qreal distThreshold = 0.2;

    for (int i = 0; i < pattern_.size(); ++i) {
        auto a = pattern_[i];
        auto b = other.pattern_[i];
        if (qAbs(a->start().x() - b->start().x()) > distThreshold ||
            qAbs(a->start().y() - b->start().y()) > distThreshold ||
            qAbs(a->end().x() - b->end().x()) > distThreshold ||
            qAbs(a->end().y() - b->end().y()) > distThreshold ||
            a->direction() != b->direction() ||
            a->angle() != b->angle()) {
            return false;
        }
    }
    return true;
}
