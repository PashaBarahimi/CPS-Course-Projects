#include "patternrecognizer.h"
#include <QDebug>
#include <QtMath>

PatternRecognizer::PatternRecognizer(QString name,
                                     AccelerometerHandler *accelerometerHandler,
                                     GyroscopeHandler *gyroscopeHandler,
                                     QObject *parent)
    : QObject(parent), name_(name), accelerometerHandler_(accelerometerHandler), gyroscopeHandler_(gyroscopeHandler)
{
    location_ = QPointF(0, 0);
    state_ = State::Stopped;
    updateAngle(Angle::Degree0);

    connect(accelerometerHandler_, &AccelerometerHandler::readingChanged, this, &PatternRecognizer::handleAccelReading);
    connect(gyroscopeHandler_, &GyroscopeHandler::readingChanged, this, &PatternRecognizer::handleGyroReading);
}

void PatternRecognizer::startRecording()
{
    if (state_ != State::Stopped)
        return;

    state_ = State::Recognizing;
    accelReadings_.clear();
    gyroReadings_.clear();
    emit patternRecognizingClearMovements();
    accelerometerHandler_->start();
    gyroscopeHandler_->start();
}

void PatternRecognizer::stopRecording()
{
    if (state_ == State::Stopped)
        return;

    state_ = State::Stopped;
    location_ = QPointF(0, 0);
    updateAngle(Angle::Degree0);

    qDebug() << "State changed to Stopped";

    accelerometerHandler_->stop();
    gyroscopeHandler_->stop();
}

void PatternRecognizer::handleGyroReading(qreal x, qreal y, qreal z)
{
    if (state_ != State::Recognizing && state_ != State::InRotation)
        return;

    gyroReadings_.append(Rotation(x, y, z));

    processGyroReadings();
}

void PatternRecognizer::processGyroReadings()
{
    const int intervalCount = gyroscopeHandler_->getDataRate() / 10;
    if (gyroReadings_.size() < intervalCount)
        return;

    qreal sumZ = 0;
    for (int i = gyroReadings_.size() - intervalCount; i < gyroReadings_.size(); ++i)
    {
        sumZ += gyroReadings_[i].z;
    }
    auto avgZ = sumZ / intervalCount;

    if (state_ == State::Recognizing && qAbs(avgZ) > 65)
    {
        qDebug() << name_ << ": " << "State changed to InRotation";
        state_ = State::InRotation;
    }
    else if (state_ == State::InRotation)
    {
        int count = 0;
        for (int i = gyroReadings_.size() - 1; i >= 0; --i)
        {
            if (qAbs(gyroReadings_[i].z) < 2)
            {
                count++;
            }
            else
            {
                break;
            }
        }

        qreal timeTreshold = 0.2;
        int countTreshold = timeTreshold * accelerometerHandler_->getDataRate();

        if (count > countTreshold)
        {
            qDebug() << name_ << ": " << "State changed to Recognizing";
            state_ = State::Recognizing;
            calculateRotation();
            gyroReadings_.clear();
            accelReadings_.clear();
        }
    }
}

void PatternRecognizer::calculateRotation()
{
    const qreal deltaTime = 1.0 / gyroscopeHandler_->getDataRate();

    Rotation totalRotation = {0.0, 0.0, 0.0};

    for (const auto& velocity : gyroReadings_) {
        totalRotation.x += velocity.x * deltaTime;
        totalRotation.y += velocity.y * deltaTime;
        totalRotation.z += velocity.z * deltaTime;

        qDebug() << "Angular velocity:" << velocity.z << "degree:" << totalRotation.z;
    }

    qDebug() << "Total Rotation (Euler Angles):" << "Yaw:" << totalRotation.x << "Pitch:" << totalRotation.y << "Roll:" << totalRotation.z;

    auto roll = std::fmod(totalRotation.z, 360.0);
    if (roll < 0)
    {
        roll += 360.0;
    }
    qDebug() << name_ << ": " << "Rotation:" << roll;
    Angle::Type rotationAngle;

    if (roll < 45)
    {
        rotationAngle = Angle::Degree0;
    }
    else if (roll < 135)
    {
        rotationAngle = Angle::Degree90;
    }
    else if (roll < 225)
    {
        rotationAngle = Angle::Degree180;
    }
    else if (roll < 315)
    {
        rotationAngle = Angle::Degree270;
    }
    else
    {
        rotationAngle = Angle::Degree0;
    }

    auto newAngle = angle_ + rotationAngle;
    updateAngle(newAngle);
}

void PatternRecognizer::handleAccelReading(qreal x, qreal y, qreal z)
{
    if (state_ != State::Recognizing && state_ != State::InMovement)
        return;

    accelReadings_.append(Acceleration(x, y, z));

    processAccelReadings();
}

void PatternRecognizer::processAccelReadings()
{
    const int intervalCount = accelerometerHandler_->getDataRate() / 10;
    if (accelReadings_.size() < intervalCount)
        return;

    qreal sumX = 0;
    qreal sumY = 0;
    for (int i = accelReadings_.size() - intervalCount; i < accelReadings_.size(); ++i)
    {
        sumX += accelReadings_[i].x;
        sumY += accelReadings_[i].y;
    }
    sumX /= intervalCount;
    sumY /= intervalCount;

    if (state_ == State::Recognizing && (sumX > 0.5 || sumY > 0.5))
    {
        qDebug() << name_ << ": " << "State changed to InMovement";
        state_ = State::InMovement;
    }
    else if (state_ == State::InMovement)
    {
        int count = 0;
        for (int i = accelReadings_.size() - 1; i >= 0; --i)
        {
            if (qAbs(accelReadings_[i].x) < 0.1 && qAbs(accelReadings_[i].y) < 0.1)
            {
                count++;
            }
            else
            {
                break;
            }
        }

        qreal timeTreshold = 0.2;
        int countTreshold = timeTreshold * accelerometerHandler_->getDataRate();

        if (count > countTreshold)
        {
            qDebug() << name_ << ": " << "State changed to Recognizing";
            state_ = State::Recognizing;
            calculateDistance();
            gyroReadings_.clear();
            accelReadings_.clear();
        }
    }
}

void PatternRecognizer::calculateDistance()
{
    qreal velocityX = 0;
    qreal velocityY = 0;
    qreal distanceX = 0;
    qreal distanceY = 0;

    qreal sumX = 0;
    qreal sumY = 0;

    const qreal deltaTime = 1.0 / accelerometerHandler_->getDataRate();

    for (const auto& reading : accelReadings_) {
        sumX += qAbs(reading.x);
        sumY += qAbs(reading.y);
    }

    bool isHorizontal = sumX > sumY;


    int gotPositive = 0;
    int gotNegative = 0;

    for (const auto& reading : accelReadings_) {
        if (isHorizontal)
        {
            velocityX += reading.x * deltaTime;
            distanceX += velocityX * deltaTime;

            // qDebug() << "accel:" << reading.x << "velocity:" << velocityX << "distance:" << distanceX;

            if (gotNegative <= 3 && reading.x > 0) { gotPositive++; }
            if (gotPositive <= 3 && reading.x < 0) { gotNegative++; }
            if (gotPositive > 3 && velocityX < 0 || gotNegative > 3 && velocityX > 0) { break; }
        }
        else
        {
            velocityY += reading.y * deltaTime;
            distanceY += velocityY * deltaTime;

            // qDebug() << "accel:" << reading.y << "velocity:" << velocityY << "distance:" << distanceY;

            if (gotNegative <= 3 && reading.y > 0) { gotPositive++; }
            if (gotPositive <= 3 && reading.y < 0) { gotNegative++; }
            if (gotPositive > 3 && velocityY < 0 || gotNegative > 3 && velocityY > 0) { break; }
        }
    }


    Direction::Type direction;

    qreal newDistanceX;
    qreal newDistanceY;

    if (angle_ == Angle::Degree90)
    {
        newDistanceY = distanceX;
        newDistanceX = -distanceY;
        isHorizontal = !isHorizontal;
    }
    else if (angle_ == Angle::Degree180)
    {
        newDistanceX = -distanceX;
        newDistanceY = -distanceY;
    }
    else if (angle_ == Angle::Degree270)
    {
        newDistanceY = -distanceX;
        newDistanceX = distanceY;
        isHorizontal = !isHorizontal;
    }
    else
    {
        newDistanceX = distanceX;
        newDistanceY = distanceY;
    }

    auto start = location_;

    qreal newX = location_.x();
    qreal newY = location_.y();

    if (isHorizontal)
    {
        newX += newDistanceX;
        direction = newDistanceX > 0 ? Direction::Right : Direction::Left;
    }
    else
    {
        newY -= newDistanceY;
        direction = newDistanceY > 0 ? Direction::Top : Direction::Bottom;
    }

    auto end = QPointF(newX, newY);
    location_ = end;

    addNewMovement(start, end, direction, angle_);

    qDebug() << "Distance X:" << newDistanceX << "Y:" << newDistanceY << "Direction:" << direction << "Angle:" << Angle::toString(angle_) << "Start:" << start << "End:" << end;
}

void PatternRecognizer::addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle)
{
    currentMovement_ = new Movement(start, end, direction, angle);
    emit patternRecognizingAddMovement(currentMovement_);
}

void PatternRecognizer::updateAngle(Angle::Type angle)
{
    angle_ = angle;
    emit patternRecognizingChangeAngle(Angle::toString(angle_));
}
