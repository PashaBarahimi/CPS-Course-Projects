#include "movementpattern.h"
#include <cmath>

bool MovementPattern::matches(const MovementPattern& other) const {
    if (pattern_.size() != other.pattern_.size()) return false;

    const qreal threshold = 0.1; // You might need to adjust the threshold

    for (int i = 0; i < pattern_.size(); ++i) {
        auto a = pattern_[i];
        auto b = other.pattern_[i];
        if (qAbs(a->start().x() - b->start().x()) > threshold ||
            qAbs(a->start().y() - b->start().y()) > threshold ||
            qAbs(a->end().x() - b->end().x()) > threshold ||
            qAbs(a->end().y() - b->end().y()) > threshold ||
            a->direction() != b->direction() ||
            a->angle() != b->angle()) {
            return false;
        }
    }
    return true;
}
