#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>

#include "accelerometerhandler.h"
#include "gyroscopehandler.h"
#include "patternrecognizer.h"

class Authenticator : public QObject
{
    Q_OBJECT
public:
    explicit Authenticator(AccelerometerHandler *accelerometerHandler,
                             GyroscopeHandler *gyroscopeHandler,
                             QObject *parent = nullptr);
    void startAuthenticating();
    void stopAuthenticating(const MovementPattern& pattern);

signals:
    void authenticationAddMovement(Movement* movement);
    void authenticationUpdateMovement(Movement* movement);
    void authenticationClearMovements();
    void authenticationSuccessful();
    void authenticationFailed(const QString& error);
    void authenticationChangeAngle(const QString& angle);

private slots:
    void addMovement(Movement *movement);
    void clearMovements();

private:
    PatternRecognizer* patternRecognizer_;
    MovementPattern recordedPattern_;

};


#endif // AUTHENTICATOR_H
