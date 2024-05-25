#include "accelerometerhandler.h"

#include <QTimer>

AccelerometerHandler::AccelerometerHandler() {
    sensor_ = new QAccelerometer(this);
    sensor_->setAccelerationMode(QAccelerometer::User);
    sensor_->setDataRate(200);

    connect(sensor_, &QAccelerometer::readingChanged, this, &AccelerometerHandler::handleReading);

    int n = 3;
    int m = 3;
    double dt = 1.0 / sensor_->dataRate();

    Eigen::MatrixXd A(n, n);
    A.setIdentity();

    Eigen::MatrixXd C(m, n);
    C.setIdentity();

    Eigen::MatrixXd Q(n, n);
    Q.setIdentity();
    Q *= 0.001;

    Eigen::MatrixXd R(m, m);
    R.setIdentity();
    R *= 0.01;

    Eigen::MatrixXd P(n, n);
    P.setIdentity();
    P *= 1;

    kf_ = new KalmanFilter(dt, A, C, Q, R, P);

    Eigen::VectorXd x0(n);
    x0.setZero();
    kf_->init(0, x0);
}

void AccelerometerHandler::start() {
    sensor_->start();
}

void AccelerometerHandler::stop() {
    sensor_->stop();
}

void AccelerometerHandler::clear() {
    readings_.clear();
}

void AccelerometerHandler::handleReading() {
    QAccelerometerReading* reading = sensor_->reading();

    Acceleration rawAccel(reading->x(), reading->y(), reading->z());
    Acceleration unbiasedAccel(rawAccel.x - readingsBias_.x,
                               rawAccel.y - readingsBias_.y,
                               rawAccel.z - readingsBias_.z);

    Acceleration filteredAccel(qAbs(unbiasedAccel.x) < threshold_ ? 0 : unbiasedAccel.x,
                               qAbs(unbiasedAccel.y) < threshold_ ? 0 : unbiasedAccel.y,
                               qAbs(unbiasedAccel.z) < threshold_ ? 0 : unbiasedAccel.z);

    readings_.append(filteredAccel);
    emit readingChanged(filteredAccel.x, filteredAccel.y, filteredAccel.z);
    emit updateAccelData(rawAccel.x, rawAccel.y, rawAccel.z,
                         filteredAccel.x, filteredAccel.y, filteredAccel.z);
}

void AccelerometerHandler::startCalibrate(int durationMs = 2000) {
    readings_.clear();
    readingsBias_ = Acceleration(0, 0, 0);
    QTimer::singleShot(durationMs, this, &AccelerometerHandler::stopCalibrate);
}

void AccelerometerHandler::stopCalibrate() {
    qreal sumX = 0;
    qreal sumY = 0;
    qreal sumZ = 0;

    for (const auto& reading : readings_) {
        sumX += reading.x;
        sumY += reading.y;
        sumZ += reading.z;
    }

    readingsBias_.x = sumX / readings_.size();
    readingsBias_.y = sumY / readings_.size();
    readingsBias_.z = sumZ / readings_.size();

    qDebug() << "Bias value of each axis - X:" << readingsBias_.x << "Y:" << readingsBias_.y << "Z:" << readingsBias_.z;

    readings_.clear();
    emit calibrationFinished(true);
}
