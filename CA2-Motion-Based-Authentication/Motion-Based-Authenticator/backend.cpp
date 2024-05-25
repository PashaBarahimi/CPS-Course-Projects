#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent} {
    patternRecorder_ = new PatternRecorder(accelerometerHandler_, gyroscopeHandler_);
    authenticator_ = new Authenticator(accelerometerHandler_, gyroscopeHandler_);

    connect(patternRecorder_, &PatternRecorder::patternRecordingAddMovement, this, &Backend::patternRecordingAddMovement);
    connect(patternRecorder_, &PatternRecorder::patternRecordingClearMovements, this, &Backend::patternRecordingClearMovements);
    connect(patternRecorder_, &PatternRecorder::patternRecordingSuccessful, this, &Backend::patternRecordingSuccessful);
    connect(patternRecorder_, &PatternRecorder::patternRecordingFailed, this, &Backend::patternRecordingFailed);
    connect(patternRecorder_, &PatternRecorder::patternRecordingChangeAngle, this, &Backend::patternRecordingChangeAngle);
    connect(authenticator_, &Authenticator::authenticationAddMovement, this, &Backend::authenticationAddMovement);
    connect(authenticator_, &Authenticator::authenticationClearMovements, this, &Backend::authenticationClearMovements);
    connect(authenticator_, &Authenticator::authenticationSuccessful, this, &Backend::authenticationSuccessful);
    connect(authenticator_, &Authenticator::authenticationFailed, this, &Backend::authenticationFailed);
    connect(authenticator_, &Authenticator::authenticationChangeAngle, this, &Backend::authenticationChangeAngle);
    connect(accelerometerHandler_, &AccelerometerHandler::calibrationFinished, this, &Backend::calibrationFinished);
    connect(accelerometerHandler_, &AccelerometerHandler::updateAccelData, this, &Backend::updateAccelData);
    connect(gyroscopeHandler_, &GyroscopeHandler::calibrationFinished, this, &Backend::calibrationFinished);
    connect(gyroscopeHandler_, &GyroscopeHandler::updateGyroData, this, &Backend::updateGyroData);
}

void Backend::startCalibration() {
    qDebug() << "Calibration started";
    int durationMs = 2000;
    accelerometerHandler_->startCalibrate(durationMs);
    gyroscopeHandler_->startCalibrate(durationMs);
}

void Backend::startPatternRecording() {
    qDebug() << "Pattern recording started";
    patternRecorder_->startRecording();
}

void Backend::stopPatternRecording() {
    qDebug() << "Pattern recording finished";
    patternRecorder_->stopRecording();
}

void Backend::startAuthentication() {
    qDebug() << "Authentication recording started";

    auto recordedPattern = patternRecorder_->getPattern();
    if (recordedPattern.getPattern().isEmpty()) {
        emit authenticationFailed("No Pattern recorded");
        return;
    }

    authenticator_->startAuthenticating();
}

void Backend::stopAuthentication() {
    qDebug() << "Authentication recording finished";

    authenticator_->stopAuthenticating(patternRecorder_->getPattern());
}

void Backend::startShowingSensors() {
    accelerometerHandler_->start();
    gyroscopeHandler_->start();
}

void Backend::stopShowingSensors() {
    accelerometerHandler_->stop();
    gyroscopeHandler_->stop();
}

