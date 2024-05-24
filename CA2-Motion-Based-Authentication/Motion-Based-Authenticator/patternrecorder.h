#ifndef PATTERNRECORDER_H
#define PATTERNRECORDER_H

#include <QObject>

#include "accelerometerhandler.h"
#include "gyroscopehandler.h"
#include "patternrecognizer.h"

class PatternRecorder : public QObject
{
    Q_OBJECT
public:
    explicit PatternRecorder(AccelerometerHandler *accelerometerHandler,
                             GyroscopeHandler *gyroscopeHandler,
                             QObject *parent = nullptr);
    void startRecording();
    void stopRecording();
    MovementPattern getPattern() const;

signals:
    void patternRecordingAddMovement(Movement *movement);
    void patternRecordingUpdateMovement(Movement *movement);
    void patternRecordingClearMovements();
    void patternRecordingSuccessful();
    void patternRecordingFailed(const QString &error);

private slots:
    void addMovement(Movement *movement);
    void clearMovements();

private:
    PatternRecognizer* patternRecognizer_;
    MovementPattern recordedPattern_;

};

#endif // PATTERNRECORDER_H
