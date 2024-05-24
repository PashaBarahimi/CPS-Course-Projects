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
    gyroReadings_.clear();
    emit patternRecordingClearMovements();
    accelerometerHandler_->start();
    gyroscopeHandler_->start();
}

void PatternRecognizer::stopRecording() {
    if (state_ == State::Stopped) return;

    state_ = State::Stopped;
    location_ = QPointF(0, 0);

    accelerometerHandler_->stop();
    gyroscopeHandler_->stop();

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

    gyroReadings_.append(Rotation(x, y, z));
}

void PatternRecognizer::processGyroReadings() {
    const qreal deltaTime = 1.0 / gyroscopeHandler_->getDataRate();

    QQuaternion totalRotation = integrateGyroReadings(gyroReadings_, deltaTime);
    QVector3D euler = totalRotation.toEulerAngles();
    double yaw = euler.y();
    double pitch = euler.x();
    double roll = euler.z();

    qDebug() << "Total Rotation (Euler Angles):" << "Yaw:" << yaw << "Pitch:" << pitch << "Roll:" << roll;

    auto rotation = std::fmod(-roll, 360.0);
    if (rotation < 0) {
        rotation += 360.0;
    }
    qDebug() << "Rotation:" << rotation;
    Angle::Type rotationAngle;

    if (rotation < 45) {
        rotationAngle = Angle::Degree0;
    } else if (rotation < 135) {
        rotationAngle = Angle::Degree90;
    } else if (rotation < 225) {
        rotationAngle = Angle::Degree180;
    } else if (rotation < 315) {
        rotationAngle = Angle::Degree270;
    } else {
        rotationAngle = Angle::Degree0;
    }

    angle_ = angle_ + rotationAngle;
    qDebug() << "Current Angle:" << Angle::toString(angle_);
}

QQuaternion PatternRecognizer::integrateGyroReadings(const QVector<Rotation>& readings, double dt) {
    QQuaternion totalRotation(1, 0, 0, 0);

    for (const auto& reading : readings) {
        qDebug() << "Gyro Reading:" << reading.x << reading.y << reading.z;

        double wx = reading.x;
        double wy = reading.y;
        double wz = reading.z;

        double angle = std::sqrt(wx * wx + wy * wy + wz * wz) * dt;

        if (angle > 0.0) {
            wx /= angle;
            wy /= angle;
            wz /= angle;
        }

        QQuaternion deltaRotation = QQuaternion::fromAxisAndAngle(wx, wy, wz, angle * (180.0 / M_PI)); // Convert angle to degrees

        totalRotation *= deltaRotation;
    }

    return totalRotation.normalized();
}

bool PatternRecognizer::authenticateMovement(const MovementPattern& pattern) const {
    return recordedPattern_.matches(pattern);
}

void PatternRecognizer::handleAccelReading(qreal x, qreal y, qreal z) {
    if (state_ == State::Stopped) return;

    accelReadings_.append(Acceleration(x, y, z));
    processAccelReadings();
}

void PatternRecognizer::processAccelReadings() {
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
        qDebug() << "State changed to InMovement";
        state_ = State::InMovement;
        processGyroReadings();
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

        // qDebug() << "Velocity: " << velocity << "Sum: " << sumX << sumY << "Count: " << count << countTreshold;

        if (count > countTreshold) {
            qDebug() << "State changed to OutMovement";
            state_ = State::OutMovement;
            calculateDistance();
            gyroReadings_.clear();
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
        sumX += qAbs(reading.x);
        sumY += qAbs(reading.y);

        qreal accelX = reading.x;
        qreal accelY = reading.y;

        velocityX += accelX * deltaTime;
        velocityY += accelY * deltaTime;

        distanceX += velocityX * deltaTime + 0.5 * accelX * deltaTime * deltaTime;
        distanceY += velocityY * deltaTime + 0.5 * accelY * deltaTime * deltaTime;
    }

    qDebug() << "Distance X:" << distanceX << "Y:" << distanceY << "Sum X:" << sumX << "Y:" << sumY;

    Direction::Type direction;

    qreal newDistanceX;
    qreal newDistanceY;

    if (angle_ == Angle::Degree90) {
        newDistanceY = -distanceX;
        newDistanceX = distanceY;
        qreal temp = sumX;
        sumX = sumY;
        sumY = temp;
    }
    else if (angle_ == Angle::Degree180) {
        newDistanceX = -distanceX;
        newDistanceY = -distanceY;
    }
    else if (angle_ == Angle::Degree270) {
        newDistanceY = distanceX;
        newDistanceX = -distanceY;
        qreal temp = sumX;
        sumX = sumY;
        sumY = temp;
    }
    else {
        newDistanceX = distanceX;
        newDistanceY = distanceY;
    }

    auto start = location_;

    qreal newX = location_.x();
    qreal newY = location_.y();

    if (sumX > sumY) {
        newX += newDistanceX;
        direction = newDistanceX > 0 ? Direction::Right : Direction::Left;
    }
    else {
        newY += newDistanceY;
        direction = newDistanceY > 0 ? Direction::Bottom : Direction::Top;
    }

    auto end = QPointF(newX, newY);
    location_ = end;


    addNewMovement(start, end, direction, angle_);

    qDebug() << "Distance X:" << distanceX << "Y:" << distanceY << "Direction:" << direction << "Angle:" << Angle::toString(angle_) << "Start:" << start << "End:" << end;
}

void PatternRecognizer::addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle) {
    currentMovement_ = new Movement(start, end, direction, angle);
    recordedPattern_.addMovement(currentMovement_);
    emit patternRecordingAddMovement(currentMovement_);
}
