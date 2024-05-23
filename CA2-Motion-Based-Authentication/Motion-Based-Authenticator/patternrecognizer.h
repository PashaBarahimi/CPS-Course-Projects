#ifndef PATTERNRECOGNIZER_H
#define PATTERNRECOGNIZER_H

#include <QObject>
#include <QAccelerometer>
#include <QTimer>
#include <QPair>

#include "movementpattern.h"
#include "accelerometerhandler.h"
#include "gyroscopehandler.h"
#include "acceleration.h"

class PatternRecognizer : public QObject
{
    Q_OBJECT
public:
    PatternRecognizer(AccelerometerHandler* accelerometerHandler, GyroscopeHandler* gyroscopeHandler, QObject *parent = nullptr);
    void startRecording();
    void stopRecording();
    QPair<qreal, qreal> calculateVelocity() const;
    MovementPattern getRecordedPattern() const;
    bool authenticateMovement(const MovementPattern &pattern) const;

signals:
    void patternRecordingAddMovement(Movement *movement);
    void patternRecordingUpdateMovement(Movement *movement);
    void patternRecordingClearMovements();
    void patternRecordingSuccessful();
    void patternRecordingFailed(const QString &error);

private slots:
    void handleAccelReading(qreal x, qreal y, qreal z);
    void handleGyroReading(qreal x, qreal y, qreal z);

private:
    void calculateDistance();
    void processReadings();
    void addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle);

private:
    AccelerometerHandler *accelerometerHandler_;
    GyroscopeHandler *gyroscopeHandler_;
    Movement *currentMovement_;
    MovementPattern recordedPattern_;
    bool isRecording_;
    bool isRecordingMovement_;
    QVector<Acceleration> readings_;
    QPointF location_;
};

#endif // PATTERNRECOGNIZER_H
