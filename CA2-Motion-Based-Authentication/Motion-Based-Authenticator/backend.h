#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlEngine>

#include "movement.h"
#include "accelerometerhandler.h"
#include "gyroscopehandler.h"
#include "patternrecognizer.h"
#include "patternrecorder.h"
#include "authenticator.h"

class Backend : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit Backend(QObject *parent = nullptr);

    Q_INVOKABLE void startCalibration();

    Q_INVOKABLE void startPatternRecording();
    Q_INVOKABLE void stopPatternRecording();

    Q_INVOKABLE void startAuthentication();
    Q_INVOKABLE void stopAuthentication();

    Q_INVOKABLE void startShowingSensors();
    Q_INVOKABLE void stopShowingSensors();

Q_SIGNALS:
    void calibrationFinished(bool success);
    void updateGyroData(qreal x, qreal y, qreal z, qreal xCal, qreal yCal, qreal zCal);
    void updateAccelData(qreal x, qreal y, qreal z, qreal xCal, qreal yCal, qreal zCal);

    void patternRecordingAddMovement(Movement* movement);
    void patternRecordingUpdateMovement(Movement* movement);
    void patternRecordingPopMovement();
    void patternRecordingClearMovements();
    void patternRecordingSuccessful();
    void patternRecordingFailed(const QString& error);
    void patternRecordingChangeAngle(const QString& angle);

    void authenticationAddMovement(Movement* movement);
    void authenticationUpdateMovement(Movement* movement);
    void authenticationPopMovement();
    void authenticationClearMovements();
    void authenticationSuccessful();
    void authenticationFailed(const QString& error);

private:
    AccelerometerHandler* accelerometerHandler_ = new AccelerometerHandler();
    GyroscopeHandler* gyroscopeHandler_ = new GyroscopeHandler();
    PatternRecognizer* patternRecognizer_;
    PatternRecorder* patternRecorder_;
    Authenticator* authenticator_;
};

#endif // BACKEND_H
