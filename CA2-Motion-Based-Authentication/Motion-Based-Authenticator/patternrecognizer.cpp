#include "patternrecognizer.h"
#include <QDebug>
#include <QtMath>

PatternRecognizer::PatternRecognizer(AccelerometerHandler* accelerometerHandler, QObject *parent)
    : QObject(parent), isRecording(false), currentMovement(nullptr) {
    accelerometerHandler_ = accelerometerHandler;
    connect(accelerometerHandler_, &AccelerometerHandler::readingChanged, this, &PatternRecognizer::handleReading);
}

void PatternRecognizer::startRecording() {
    if (isRecording) return;
    isRecording = true;
    recordedPattern = MovementPattern();
    readings.clear();
    emit patternRecordingClearMovements();
    accelerometerHandler_->start();
}

void PatternRecognizer::stopRecording() {
    if (!isRecording) return;
    isRecording = false;
    accelerometerHandler_->stop();
    processReadings();
    if (recordedPattern.getPattern().isEmpty()) {
        emit patternRecordingFailed("No movements recorded");
    } else {
        emit patternRecordingSuccessful();
    }
}

MovementPattern PatternRecognizer::getRecordedPattern() const {
    return recordedPattern;
}

bool PatternRecognizer::authenticateMovement(const MovementPattern& pattern) const {
    return recordedPattern.matches(pattern);
}

void PatternRecognizer::handleReading() {
    if (!isRecording) return;
    auto reading = accelerometerHandler_->sensor_->reading();
    readings.append(reading);
    processReadings();
}

void PatternRecognizer::processReadings() {
    const int intervalCount = 20;
    if (readings.size() < intervalCount) return;

    qreal sumX = 0;
    qreal sumY = 0;
    for (int i = readings.size() - intervalCount; i < readings.size(); ++i) {
        sumX += readings[i]->x();
        sumY += readings[i]->y();
    }
    sumX /= intervalCount;
    sumY /= intervalCount;

    if (!isRecordingMovement && (qAbs(sumX) > 2 || qAbs(sumY) > 2)) {
        isRecordingMovement = true;
    } else if (isRecordingMovement && (qAbs(sumX) < 2 && qAbs(sumY) < 02)) {
        isRecordingMovement = false;
        calculateDistance();
        readings.clear();
    }
}

void PatternRecognizer::calculateDistance() {
    qreal velocityX = 0;
    qreal velocityY = 0;
    qreal distanceX = 0;
    qreal distanceY = 0;

    const qreal deltaTime = 1 / static_cast<qreal>(accelerometerHandler_->getDataRate());
    qDebug() << "DataRate" << accelerometerHandler_->getDataRate() << "Delta time:" << deltaTime;
    qDebug() << "count of readings:" << readings.size();

    for (auto reading : readings) {
        qreal accelX = reading->x();
        qreal accelY = reading->y();

        velocityX += accelX * deltaTime;
        velocityY += accelY * deltaTime;

        distanceX += velocityX * deltaTime + 0.5 * accelX * deltaTime * deltaTime;
        distanceY += velocityY * deltaTime + 0.5 * accelY * deltaTime * deltaTime;
    }

    Direction::Type direction;

    auto start = QPointF {x_, y_};
    if (qAbs(distanceX) > qAbs(distanceY)) {
        x_ += distanceX;
        direction = distanceX > 0 ? Direction::Right : Direction::Left;
    }
    else {
        y_ += distanceY;
        direction = distanceY > 0 ? Direction::Bottom : Direction::Top;
    }
    auto end = QPointF {x_, y_};


    addNewMovement(start, end, direction, Angle::Degree0);

    qDebug() << "Distance X:" << distanceX << "Y:" << distanceY << "Direction:" << direction;


}

void PatternRecognizer::addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle) {
    currentMovement = new Movement(start, end, direction, angle);
    recordedPattern.addMovement(currentMovement);
    emit patternRecordingAddMovement(currentMovement);
}
