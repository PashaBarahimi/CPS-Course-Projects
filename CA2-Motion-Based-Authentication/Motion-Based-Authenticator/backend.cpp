#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent} {
    patternRecognizer_ = new PatternRecognizer(accelerometerHandler_, gyroscopeHandler_);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingAddMovement, this, &Backend::patternRecordingAddMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingUpdateMovement, this, &Backend::patternRecordingUpdateMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingClearMovements, this, &Backend::patternRecordingClearMovements);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingSuccessful, this, &Backend::patternRecordingSuccessful);
    connect(patternRecognizer_, &PatternRecognizer::patternRecordingFailed, this, &Backend::patternRecordingFailed);
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
    patternRecognizer_->startRecording();
}

void Backend::stopPatternRecording() {
    qDebug() << "Pattern recording finished";
    patternRecognizer_->stopRecording();
}

void Backend::startAuthentication() {
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
    gyroscopeHandler_->start();
}

void Backend::stopShowingSensors() {
    accelerometerHandler_->stop();
    gyroscopeHandler_->stop();
}

