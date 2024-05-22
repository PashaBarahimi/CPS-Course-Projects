#ifndef ACCELEROMETERHANDLER_H
#define ACCELEROMETERHANDLER_H

#include <QCoreApplication>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QObject>
#include <QDebug>

class AccelerometerHandler : public QObject {
    Q_OBJECT
public:
    AccelerometerHandler();
    void startCalibrate();
    void stopCalibrate();
    void startReading(int durationMs);

Q_SIGNALS:
    void calibrationFinished(bool success);

private slots:
    void handleReading();

private:
    void start();
    void stop();
    void clear();

private:
    QAccelerometer *sensor_;
    QVector<qreal> readingsX;
    QVector<qreal> readingsY;
    QVector<qreal> readingsZ;
    qreal xBias;
    qreal yBias;
    qreal zBias;
};

#endif // ACCELEROMETERHANDLER_H
