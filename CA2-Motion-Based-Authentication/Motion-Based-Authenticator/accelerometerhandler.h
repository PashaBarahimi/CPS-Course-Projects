#ifndef ACCELEROMETERHANDLER_H
#define ACCELEROMETERHANDLER_H

#include <QCoreApplication>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QObject>
#include <QDebug>

#include "acceleration.h"
#include "kalmanfilter.h"

class AccelerometerHandler : public QObject {
    Q_OBJECT
public:
    AccelerometerHandler();
    void startCalibrate();
    void stopCalibrate();
    void start();
    void stop();
    void clear();
    int getDataRate() const { return sensor_->dataRate(); }

Q_SIGNALS:
    void calibrationFinished(bool success);
    void readingChanged(qreal x, qreal y, qreal z);
    void updateAccelData(qreal x, qreal y, qreal z, qreal xCal, qreal yCal, qreal zCal);

private slots:
    void handleReading();

private:
    QVector<Acceleration> readings_;
    Acceleration readingsBias_;
    QAccelerometer *sensor_;
    KalmanFilter *kf_;
    const qreal threshold_ = 0.01;
};

#endif // ACCELEROMETERHANDLER_H
