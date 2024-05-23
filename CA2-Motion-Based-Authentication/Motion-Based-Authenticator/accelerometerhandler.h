#ifndef ACCELEROMETERHANDLER_H
#define ACCELEROMETERHANDLER_H

#include <QCoreApplication>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QObject>
#include <QDebug>

#include "acceleration.h"

class AccelerometerHandler : public QObject {
    Q_OBJECT
public:
    AccelerometerHandler();
    void startCalibrate();
    void stopCalibrate();
    void start();
    void stop();
    void clear();
    QVector<Acceleration> getReadings() const { return readings; }
    int getDataRate() const { return sensor_->dataRate(); }
    QAccelerometer *sensor_;

Q_SIGNALS:
    void calibrationFinished(bool success);
    void readingChanged();
    void updateAccelData(qreal x, qreal y, qreal z, qreal xCal, qreal yCal, qreal zCal);

private slots:
    void handleReading();

private:


private:
    QVector<Acceleration> readings;
    Acceleration readingsBias;
};

#endif // ACCELEROMETERHANDLER_H
