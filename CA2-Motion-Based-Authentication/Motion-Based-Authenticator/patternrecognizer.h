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
    enum State
    {
        Stopped,
        InRotation,
        InMovement,
        Recognizing
    };
    Q_ENUM(State)

    PatternRecognizer(QString name, AccelerometerHandler *accelerometerHandler, GyroscopeHandler *gyroscopeHandler, QObject *parent = nullptr);
    void startRecording();
    void stopRecording();
    QPair<qreal, qreal> calculateVelocity() const;

signals:
    void patternRecognizingAddMovement(Movement *movement);
    void patternRecognizingUpdateMovement(Movement *movement);
    void patternRecognizingClearMovements();
    void patternRecognizingChangeAngle(const QString &angle);

private slots:
    void handleAccelReading(qreal x, qreal y, qreal z);
    void handleGyroReading(qreal x, qreal y, qreal z);

private:
    void calculateDistance();
    void calculateRotation();
    QQuaternion integrateGyroReadings(const QVector<Rotation> &readings, double dt);
    void processAccelReadings();
    void processGyroReadings();
    void addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle);
    void updateAngle(Angle::Type angle);

private:
    AccelerometerHandler *accelerometerHandler_;
    GyroscopeHandler *gyroscopeHandler_;
    Movement *currentMovement_;
    QVector<Acceleration> accelReadings_;
    QVector<Rotation> gyroReadings_;
    QPointF location_;
    State state_;
    Angle::Type angle_;
    QString name_;
};

#endif // PATTERNRECOGNIZER_H
