#ifndef PATTERNRECOGNIZER_H
#define PATTERNRECOGNIZER_H

#include <QObject>
#include <QAccelerometer>
#include <QTimer>
#include <QPair>
#include <QQuaternion>

#include "movementpattern.h"
#include "accelerometerhandler.h"
#include "gyroscopehandler.h"
#include "acceleration.h"
#include "rotation.h"
#include "angle.h"

class PatternRecognizer : public QObject
{
    Q_OBJECT
public:
    enum State {
        Stopped,
        InMovement,
        OutMovement
    };
    Q_ENUM(State)

    PatternRecognizer(AccelerometerHandler* accelerometerHandler, GyroscopeHandler* gyroscopeHandler, QObject *parent = nullptr);
    void startRecording();
    void stopRecording();
    QPair<qreal, qreal> calculateVelocity() const;

signals:
    void patternRecognizingAddMovement(Movement *movement);
    void patternRecognizingUpdateMovement(Movement *movement);
    void patternRecognizingClearMovements();

private slots:
    void handleAccelReading(qreal x, qreal y, qreal z);
    void handleGyroReading(qreal x, qreal y, qreal z);

private:
    void calculateDistance();
    QQuaternion integrateGyroReadings(const QVector<Rotation>& readings, double dt);
    void processAccelReadings();
    void processGyroReadings();
    void addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle);

private:
    AccelerometerHandler *accelerometerHandler_;
    GyroscopeHandler *gyroscopeHandler_;
    Movement *currentMovement_;
    QVector<Acceleration> accelReadings_;
    QVector<Rotation> gyroReadings_;
    QPointF location_;
    State state_;
    Angle::Type angle_;
};

#endif // PATTERNRECOGNIZER_H
