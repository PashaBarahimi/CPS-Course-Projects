#include <QTimer>

#include "accelerometerhandler.h"

AccelerometerHandler::AccelerometerHandler() {
    sensor_ = new QAccelerometer(this);
    sensor_->setAccelerationMode(QAccelerometer::User);
    sensor_->setDataRate(100);

    connect(sensor_, &QAccelerometer::readingChanged, this, &AccelerometerHandler::handleReading);
    connect(sensor_, &QAccelerometer::readingChanged, this, &AccelerometerHandler::readingChanged);
}

void AccelerometerHandler::start() {
    sensor_->start();
}

void AccelerometerHandler::stop() {
    sensor_->stop();
}

void AccelerometerHandler::clear() {
    readings.clear();
}

void AccelerometerHandler::handleReading() {
    QAccelerometerReading *reading = sensor_->reading();
    Acceleration acceleration(reading->x(), reading->y(), reading->z());
    readings.append(acceleration);
    emit updateAccelData(acceleration.x, acceleration.y, acceleration.z,
                        acceleration.x - readingsBias.x, acceleration.y - readingsBias.y, acceleration.z - readingsBias.z);
}

void AccelerometerHandler::startCalibrate() {
    const int durationMs = 1000;
    QTimer::singleShot(durationMs, this, &AccelerometerHandler::stopCalibrate);
}

void AccelerometerHandler::stopCalibrate() {
    qreal sumX = 0;
    qreal sumY = 0;
    qreal sumZ = 0;

    for (const auto& reading : readings) {
        sumX += reading.x;
        sumY += reading.y;
        sumZ += reading.z;
    }

    readingsBias.x = sumX / readings.size();
    readingsBias.y = sumY / readings.size();
    readingsBias.z = sumZ / readings.size();

    qDebug() << "Bias value of each axis - X:" << readingsBias.x << "Y:" << readingsBias.y << "Z:" << readingsBias.z;
    emit calibrationFinished(true);
}
