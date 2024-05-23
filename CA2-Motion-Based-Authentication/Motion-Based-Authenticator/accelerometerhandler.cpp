#include <QTimer>

#include "accelerometerhandler.h"

AccelerometerHandler::AccelerometerHandler() {
    sensor_ = new QAccelerometer(this);
    sensor_->setAccelerationMode(QAccelerometer::User);
    sensor_->setDataRate(100);

    connect(sensor_, &QAccelerometer::readingChanged, this, &AccelerometerHandler::handleReading);
    connect(sensor_, &QAccelerometer::readingChanged, this, &AccelerometerHandler::readingChanged);


    // Initialize the Kalman filter
    int n = 3; // Number of states (x, y, z)
    int m = 3; // Number of measurements (accelerometer outputs x, y, z)
    double dt = 1.0 / sensor_->dataRate(); // Time step based on sensor data rate

    // System dynamics matrix (A)
    Eigen::MatrixXd A(n, n);
    A.setIdentity();

    // Output matrix (C)
    Eigen::MatrixXd C(m, n);
    C.setIdentity();

    // Process noise covariance (Q)
    Eigen::MatrixXd Q(n, n);
    Q.setIdentity();
    Q *= 0.001; // Tunable

    // Measurement noise covariance (R)
    Eigen::MatrixXd R(m, m);
    R.setIdentity();
    R *= 0.01; // Tunable

    // Estimate error covariance (P)
    Eigen::MatrixXd P(n, n);
    P.setIdentity();
    P *= 1;

    kf = new KalmanFilter(dt, A, C, Q, R, P);

    // Initialize the filter with an initial state (e.g., zero initial state)
    Eigen::VectorXd x0(n);
    x0.setZero();
    kf->init(0, x0);
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

    // Update the Kalman filter with the new measurement
    Eigen::VectorXd y(3);
    y << reading->x(), reading->y(), reading->z();
    kf->update(y);

    // Get the filtered state
    Eigen::VectorXd filteredState = kf->state();

    Acceleration filteredAccel(filteredState(0), filteredState(1), filteredState(2));
    readings.append(filteredAccel);

    emit updateAccelData(filteredState(0), filteredState(1), filteredState(2),
                         filteredState(0) - readingsBias.x, filteredState(1) - readingsBias.y, filteredState(2) - readingsBias.z);
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
