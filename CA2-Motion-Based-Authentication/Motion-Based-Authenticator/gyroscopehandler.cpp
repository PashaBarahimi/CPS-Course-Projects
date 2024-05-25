#include <QTimer>

#include "gyroscopehandler.h"

GyroscopeHandler::GyroscopeHandler() {
    sensor_ = new QGyroscope(this);
    sensor_->setDataRate(100);

    connect(sensor_, &QGyroscope::readingChanged, this, &GyroscopeHandler::handleReading);

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

void GyroscopeHandler::start() {
    sensor_->start();
}

void GyroscopeHandler::stop() {
    sensor_->stop();
}

void GyroscopeHandler::clear() {
    readings_.clear();
}

void GyroscopeHandler::handleReading() {
    QGyroscopeReading *reading = sensor_->reading();

    Rotation rawGyro(reading->x(), reading->y(), reading->z());
    Rotation unbiasedGyro(rawGyro.x - readingsBias_.x,
                          rawGyro.y - readingsBias_.y,
                          rawGyro.z - readingsBias_.z);

    Eigen::VectorXd y(3);
    y << unbiasedGyro.x, unbiasedGyro.y, unbiasedGyro.z;
    kf_->update(y);

    Eigen::VectorXd filteredState = kf_->state();

    Rotation filteredGyro(qAbs(unbiasedGyro.x) < threshold_ ? 0 : unbiasedGyro.x,
                          qAbs(unbiasedGyro.y) < threshold_ ? 0 : unbiasedGyro.y,
                          qAbs(unbiasedGyro.z) < threshold_ ? 0 : unbiasedGyro.z);

    readings_.append(filteredGyro);
    emit readingChanged(filteredGyro.x, filteredGyro.y, filteredGyro.z);
    emit updateGyroData(rawGyro.x, rawGyro.y, rawGyro.z,
                        filteredGyro.x, filteredGyro.y, filteredGyro.z);
}

void GyroscopeHandler::startCalibrate(int durationMs = 2000) {
    readings_.clear();
    readingsBias_ = Rotation(0, 0, 0);
    QTimer::singleShot(durationMs, this, &GyroscopeHandler::stopCalibrate);
}

void GyroscopeHandler::stopCalibrate() {
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
