#ifndef PATTERNRECOGNIZER_H
#define PATTERNRECOGNIZER_H

#include <QObject>
#include <QAccelerometer>
#include "movementpattern.h"
#include "accelerometerhandler.h"

class PatternRecognizer : public QObject {
    Q_OBJECT
public:
    PatternRecognizer(AccelerometerHandler* accelerometerHandler, QObject *parent = nullptr);
    void startRecording();
    // void stopRecording();
    MovementPattern getRecordedPattern() const;
    // bool authenticateMovement(const MovementPattern& pattern) const;

private slots:
    // void handleReading();

private:
    AccelerometerHandler *accelerometerHandler_;
    MovementPattern recordedPattern;
    bool isRecording;
    QVector<QAccelerometerReading> readings;
    // void processReadings();
};

#endif // PATTERNRECOGNIZER_H
