#ifndef PATTERNRECOGNIZER_H
#define PATTERNRECOGNIZER_H

#include <QObject>
#include <QAccelerometer>
#include <QTimer>
#include "movementpattern.h"
#include "accelerometerhandler.h"

class PatternRecognizer : public QObject
{
    Q_OBJECT
public:
    PatternRecognizer(AccelerometerHandler *accelerometerHandler, QObject *parent = nullptr);
    void startRecording();
    void stopRecording();
    MovementPattern getRecordedPattern() const;
    bool authenticateMovement(const MovementPattern &pattern) const;

signals:
    void patternRecordingAddMovement(Movement *movement);
    void patternRecordingUpdateMovement(Movement *movement);
    void patternRecordingClearMovements();
    void patternRecordingSuccessful();
    void patternRecordingFailed(const QString &error);

private slots:
    void handleReading();

private:
    AccelerometerHandler *accelerometerHandler_;
    MovementPattern recordedPattern;
    bool isRecording;
    bool isRecordingMovement;
    QVector<QAccelerometerReading*> readings;
    QTimer *recordingTimer;
    Movement *currentMovement;

    qreal x_ = 0;
    qreal y_ = 0;

    void calculateDistance();
    void processReadings();
    void addNewMovement(QPointF start, QPointF end, Direction::Type direction, Angle::Type angle);
};

#endif // PATTERNRECOGNIZER_H
