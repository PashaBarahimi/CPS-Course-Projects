#ifndef GYROSCOPEHANDLER_H
#define GYROSCOPEHANDLER_H

#include <QCoreApplication>
#include <QGyroscope>
#include <QGyroscopeReading>
#include <QObject>
#include <QDebug>

#include "rotation.h"
#include "kalmanfilter.h"

class GyroscopeHandler : public QObject {
    Q_OBJECT
public:
    GyroscopeHandler();
    void startCalibrate(int durationMs);
    void stopCalibrate();
    void start();
    void stop();
    void clear();
    int getDataRate() const { return sensor_->dataRate(); }

Q_SIGNALS:
    void calibrationFinished(bool success);
    void readingChanged(qreal x, qreal y, qreal z);
    void updateGyroData(qreal x, qreal y, qreal z, qreal xCal, qreal yCal, qreal zCal);

private slots:
    void handleReading();

private:
    QVector<Rotation> readings_;
    Rotation readingsBias_;
    QGyroscope *sensor_;
    KalmanFilter *kf_;
    const qreal threshold_ = 0.5;
};

#endif // GYROSCOPEHANDLER_H
