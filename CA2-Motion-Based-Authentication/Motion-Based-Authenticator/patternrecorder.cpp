#include "patternrecorder.h"

PatternRecorder::PatternRecorder(AccelerometerHandler* accelerometerHandler,
                                 GyroscopeHandler* gyroscopeHandler,
                                 QObject* parent)
    : QObject{parent} {
    patternRecognizer_ = new PatternRecognizer("Recorder Recognizer", accelerometerHandler, gyroscopeHandler);

    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingAddMovement, this, &PatternRecorder::patternRecordingAddMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingAddMovement, this, &PatternRecorder::addMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingClearMovements, this, &PatternRecorder::clearMovements);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingUpdateMovement, this, &PatternRecorder::patternRecordingUpdateMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingClearMovements, this, &PatternRecorder::patternRecordingClearMovements);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingChangeAngle, this, &PatternRecorder::patternRecordingChangeAngle);
}

void PatternRecorder::startRecording() {
    patternRecognizer_->startRecording();
}

void PatternRecorder::stopRecording() {
    patternRecognizer_->stopRecording();

    if (recordedPattern_.getPattern().isEmpty()) {
        emit patternRecordingFailed("No movements recorded");
    }
    else {
        emit patternRecordingSuccessful();
    }
}

void PatternRecorder::addMovement(Movement* movement) {
    recordedPattern_.addMovement(movement);
}

void PatternRecorder::clearMovements() {
    recordedPattern_ = MovementPattern();
}

MovementPattern PatternRecorder::getPattern() const {
    return recordedPattern_;
}
