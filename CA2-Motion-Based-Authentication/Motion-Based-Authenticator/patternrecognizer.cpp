#include "patternrecognizer.h"
#include <QDebug>
#include <QtMath>

PatternRecognizer::PatternRecognizer(AccelerometerHandler* accelerometerHandler,
                                     GyroscopeHandler* gyroscopeHandler,
                                     QObject *parent)
    : QObject(parent), accelerometerHandler_(accelerometerHandler), gyroscopeHandler_(gyroscopeHandler),
    isRecording_(false), isRecordingMovement_(false) {
    connect(accelerometerHandler_, &AccelerometerHandler::readingChanged, this, &PatternRecognizer::handleAccelReading);
    connect(gyroscopeHandler_, &GyroscopeHandler::readingChanged, this, &PatternRecognizer::handleGyroReading);
}

void PatternRecognizer::startRecording() {
    if (isRecording_) return;
    isRecording_ = true;

    recordedPattern_ = MovementPattern();
    readings_.clear();
    emit patternRecordingClearMovements();
    accelerometerHandler_->start();
}

void PatternRecognizer::stopRecording() {
    if (!isRecording_) return;
    isRecording_ = false;

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
    if (!isRecording_) return;



}

bool PatternRecognizer::authenticateMovement(const MovementPattern& pattern) const {
    return recordedPattern_.matches(pattern);
}

void PatternRecognizer::handleAccelReading(qreal x, qreal y, qreal z) {
    if (!isRecording_) return;
    readings_.append(Acceleration(x, y, z));
    processReadings();
}

void PatternRecognizer::processReadings() {
    const int intervalCount = accelerometerHandler_->getDataRate() / 10;
    if (readings_.size() < intervalCount) return;

    qreal sumX = 0;
    qreal sumY = 0;
    for (int i = readings_.size() - intervalCount; i < readings_.size(); ++i) {
        sumX += readings_[i].x;
        sumY += readings_[i].y;
    }
    sumX /= intervalCount;
    sumY /= intervalCount;

    if (!isRecordingMovement_ && (sumX > 0.5 || sumY > 0.5)) {
        qDebug() << "Recording movement";
        isRecordingMovement_ = true;
    } else if (isRecordingMovement_) {
        auto velocity = calculateVelocity();

        int count = 0;
        for (int i = readings_.size() - 1; i >= 0; --i) {
            if (qAbs(readings_[i].x) < 0.1 && qAbs(readings_[i].y) < 0.1) {
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

            isRecordingMovement_ = false;
            calculateDistance();
            readings_.clear();
        }
    }
}

QPair<qreal, qreal> PatternRecognizer::calculateVelocity() const {
    qreal velocityX = 0;
    qreal velocityY = 0;
    const qreal deltaTime = 1.0 / accelerometerHandler_->getDataRate();

    for (auto reading : readings_) {
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

    for (auto reading : readings_) {
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
