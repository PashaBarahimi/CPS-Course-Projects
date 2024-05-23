#include "patternrecognizer.h"
#include <QDebug>
#include <QtMath>

PatternRecognizer::PatternRecognizer(AccelerometerHandler* accelerometerHandler,
                                     GyroscopeHandler* gyroscopeHandler,
                                     QObject *parent)
    : QObject(parent), accelerometerHandler_(accelerometerHandler), gyroscopeHandler_(gyroscopeHandler) {
    location_ = QPointF(0, 0);
    state_ = State::Stopped;
    angle_ = Angle::Degree0;

    connect(accelerometerHandler_, &AccelerometerHandler::readingChanged, this, &PatternRecognizer::handleAccelReading);
    connect(gyroscopeHandler_, &GyroscopeHandler::readingChanged, this, &PatternRecognizer::handleGyroReading);
}

void PatternRecognizer::startRecording() {
    if (state_ != State::Stopped) return;

    state_ = State::OutMovement;
    recordedPattern_ = MovementPattern();
    accelReadings_.clear();
    emit patternRecordingClearMovements();
    accelerometerHandler_->start();
}

void PatternRecognizer::stopRecording() {
    if (state_ == State::Stopped) return;

    state_ = State::Stopped;
    location_ = QPointF(0, 0);

    accelerometerHandler_->stop();

    if (recordedPattern_.getPattern().isEmpty()) {
        emit patternRecordingFailed("No movements recorded");
    } else {
        emit patternRecordingSuccessful();
    }
}

MovementPattern PatternRecognizer::getRecordedPattern() const {
    return recordedPattern_;
}

void PatternRecognizer::handleGyroReading(qreal x, qreal y, qreal z) {
    if (state_ != State::OutMovement) return;

}

bool PatternRecognizer::authenticateMovement(const MovementPattern& pattern) const {
    return recordedPattern_.matches(pattern);
}

void PatternRecognizer::handleAccelReading(qreal x, qreal y, qreal z) {
    if (state_ == State::Stopped) return;

    accelReadings_.append(Acceleration(x, y, z));
    processReadings();
}

void PatternRecognizer::processReadings() {
    const int intervalCount = accelerometerHandler_->getDataRate() / 10;
    if (accelReadings_.size() < intervalCount) return;

    qreal sumX = 0;
    qreal sumY = 0;
    for (int i = accelReadings_.size() - intervalCount; i < accelReadings_.size(); ++i) {
        sumX += accelReadings_[i].x;
        sumY += accelReadings_[i].y;
    }
    sumX /= intervalCount;
    sumY /= intervalCount;

    if (state_ == State::OutMovement && (sumX > 0.5 || sumY > 0.5)) {
        qDebug() << "Recording movement";
        state_ = State::InMovement;
    } else if (state_ == State::InMovement) {
        auto velocity = calculateVelocity();

        int count = 0;
        for (int i = accelReadings_.size() - 1; i >= 0; --i) {
            if (qAbs(accelReadings_[i].x) < 0.1 && qAbs(accelReadings_[i].y) < 0.1) {
                count++;
            } else {
                break;
            }
        }

        qreal timeTreshold = 0.2;
        int countTreshold = timeTreshold * accelerometerHandler_->getDataRate();

        qDebug() << "Velocity: " << velocity << "Sum: " << sumX << sumY << "Count: " << count << countTreshold;

        if (count > countTreshold) {
            qDebug() << "Movement ended";

            state_ = State::OutMovement;
            calculateDistance();
            accelReadings_.clear();
        }
    }
}

QPair<qreal, qreal> PatternRecognizer::calculateVelocity() const {
    qreal velocityX = 0;
    qreal velocityY = 0;
    const qreal deltaTime = 1.0 / accelerometerHandler_->getDataRate();

    for (auto reading : accelReadings_) {
        qreal accelX = reading.x;
        qreal accelY = reading.y;

        velocityX += accelX * deltaTime;
        velocityY += accelY * deltaTime;
    }

    return {velocityX, velocityY};
}

void PatternRecognizer::calculateDistance() {
    qreal velocityX = 0;
    qreal velocityY = 0;
    qreal distanceX = 0;
    qreal distanceY = 0;

    qreal sumX = 0;
    qreal sumY = 0;

    const qreal deltaTime = 1.0 / accelerometerHandler_->getDataRate();

    for (auto reading : accelReadings_) {
        qDebug() << "Reading:" << reading.x << reading.y;

        sumX += qAbs(reading.x);
        sumY += qAbs(reading.y);

        qreal accelX = reading.x;
        qreal accelY = reading.y;

        velocityX += accelX * deltaTime;
        velocityY += accelY * deltaTime;

        distanceX += velocityX * deltaTime + 0.5 * accelX * deltaTime * deltaTime;
        distanceY += velocityY * deltaTime + 0.5 * accelY * deltaTime * deltaTime;
    }

    qDebug() << "Sum X:" << sumX << "Y:" << sumY;

    Direction::Type direction;

    auto start = location_;

    qreal newX = location_.x();
    qreal newY = location_.y();

    if (sumX > sumY) {
        newX += distanceX;
        direction = distanceX > 0 ? Direction::Right : Direction::Left;
    }
    else {
        newY += distanceY;
        direction = distanceY > 0 ? Direction::Bottom : Direction::Top;
    }

    auto end = QPointF(newX, newY);
    location_ = end;

    addNewMovement(start, end, direction, Angle::Degree0);

    qDebug() << "Distance X:" << distanceX << "Y:" << distanceY << "Direction:" << direction;
}

void PatternRecognizer::addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle) {
    currentMovement_ = new Movement(start, end, direction, angle);
    recordedPattern_.addMovement(currentMovement_);
    emit patternRecordingAddMovement(currentMovement_);
}
