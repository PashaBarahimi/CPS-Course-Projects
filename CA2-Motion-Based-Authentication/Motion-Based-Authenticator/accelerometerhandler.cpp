#include <QTimer>

#include "backend.h"
#include "accelerometerhandler.h"

AccelerometerHandler::AccelerometerHandler() {
    sensor_ = new QAccelerometer(this);
    sensor_->setAccelerationMode(QAccelerometer::User);
    connect(sensor_, &QAccelerometer::readingChanged, this, &AccelerometerHandler::handleReading);
}

void AccelerometerHandler::start() {
    sensor_->start();
}

void AccelerometerHandler::stop() {
    sensor_->stop();
}

void AccelerometerHandler::clear() {
    readingsX.clear();
    readingsY.clear();
    readingsZ.clear();
}

void AccelerometerHandler::handleReading() {
    QAccelerometerReading *reading = sensor_->reading();
    readingsX.append(reading->x());
    readingsY.append(reading->y());
    readingsZ.append(reading->z());
}

void AccelerometerHandler::startCalibrate() {
    const int durationMs = 2000;
    start();
    QTimer::singleShot(durationMs, this, &AccelerometerHandler::stopCalibrate);
}

void AccelerometerHandler::stopCalibrate() {
    stop();

    qreal sumX = std::accumulate(readingsX.begin(), readingsX.end(), 0.0);
    qreal sumY = std::accumulate(readingsY.begin(), readingsY.end(), 0.0);
    qreal sumZ = std::accumulate(readingsZ.begin(), readingsZ.end(), 0.0);

    xBias = sumX / readingsX.size();
    yBias = sumY / readingsY.size();
    zBias = sumZ / readingsZ.size();

    qDebug() << "Bias value of each axis - X:" << xBias << "Y:" << yBias << "Z:" << zBias;
    emit calibrationFinished(true);
}
