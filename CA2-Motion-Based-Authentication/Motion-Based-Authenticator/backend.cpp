#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent} {
    patternRecognizer_ = new PatternRecognizer(accelerometerHandler_);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingAddMovement, this, &Backend::patternRecordingAddMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingUpdateMovement, this, &Backend::patternRecordingUpdateMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingClearMovements, this, &Backend::patternRecordingClearMovements);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingSuccessful, this, &Backend::patternRecordingSuccessful);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingFailed, this, &Backend::patternRecordingFailed);
    connect(accelerometerHandler_, &AccelerometerHandler::calibrationFinished, this, &Backend::calibrationFinished);
    connect(accelerometerHandler_, &AccelerometerHandler::updateAccelData, this, &Backend::updateAccelData);
}

void Backend::startCalibration() {
    qDebug() << "Calibration started";
    accelerometerHandler_->startCalibrate();
}

void Backend::startPatternRecording() {
    qDebug() << "Pattern recording started";
    patternRecognizer_->startRecording();
}

void Backend::stopPatternRecording() {
    qDebug() << "Pattern recording finished";
    patternRecognizer_->stopRecording();
}

void Backend::startAuthentication() {
    // Implement similar to startPatternRecording but store the pattern for comparison
    qDebug() << "Authentication recording started";
    patternRecognizer_->startRecording();
}

void Backend::stopAuthentication() {
    qDebug() << "Authentication recording finished";
    patternRecognizer_->stopRecording();
    MovementPattern authenticationPattern = patternRecognizer_->getRecordedPattern();
    if (patternRecognizer_->authenticateMovement(authenticationPattern)) {
        emit authenticationSuccessful();
    } else {
        emit authenticationFailed("Pattern mismatch");
    }
}

void Backend::startShowingSensors() {
    accelerometerHandler_->start();
}

void Backend::stopShowingSensors() {
    accelerometerHandler_->stop();
}


// Signals
//
//
// updateGyroData(qreal, qreal, qreal, qreal, qreal, qreal) is to update the sensor values in the Sensor's page
// we can emit this signal with the real rate of sensor reading or decrase the rate as it is only used
// to show the user and not used for any calculations
// the first 3 values are for actual x, y, z
// the next 3 values are for calibrated x, y, z
// obviously, the calibrated and actual values are the same before calibration
// after calibration, the calibrated values should be around 0 when the device is in a steady state
//
// updateAccelData(...) same as above but for accelerometer
//
// patternRecordingAddMovement(Movement*) called when a new movement is recognized (look out for memory leaks =) )
//
// patternRecordingUpdateMovement(Movement*) called when we want to change the last movement recognized. It is used
// to provide live UI updates. For example, the phone is moved 10cm to the top and we recognize it as a new movement
// using the above signal. Then, the phone is again moved in the same direction and it is not recognized as new movement
// and we should update the previous movement using this signal
//
// patternRecordingPopMovement() pops the last movement. No idea if we gonna need it or not (maybe for a wrong movement detection)
//
// patternRecordingClearMovements() is used to clear the history which we should do when a new recording is started
//
// patternRecordingSuccessful() is obvious =)
//
// patternRecordingFailed(QString) obvoius but with an error message (we're gonna use it for authentication such as Pattern Mismatch
// but no idea if we need it here or not)
//
// The next 6 are for authentication and same as the above 6 signals
