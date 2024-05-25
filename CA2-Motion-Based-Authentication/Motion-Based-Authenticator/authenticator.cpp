#include "authenticator.h"

Authenticator::Authenticator(AccelerometerHandler* accelerometerHandler,
                             GyroscopeHandler* gyroscopeHandler,
                             QObject* parent)
    : QObject{parent} {
    patternRecognizer_ = new PatternRecognizer("Authenticator Recognizer", accelerometerHandler, gyroscopeHandler);

    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingAddMovement, this, &Authenticator::authenticationAddMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingAddMovement, this, &Authenticator::addMovement);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingClearMovements, this, &Authenticator::clearMovements);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingClearMovements, this, &Authenticator::authenticationClearMovements);
    connect(patternRecognizer_, &PatternRecognizer::patternRecognizingChangeAngle, this, &Authenticator::authenticationChangeAngle);
}

void Authenticator::startAuthenticating() {
    patternRecognizer_->startRecording();
}

void Authenticator::stopAuthenticating(const MovementPattern& pattern) {
    patternRecognizer_->stopRecording();

    if (recordedPattern_.getPattern().isEmpty()) {
        emit authenticationFailed("No movements recorded");
    }
    else {
        pattern.matches(recordedPattern_) ? emit authenticationSuccessful() : emit authenticationFailed("Pattern does not match");
    }
}

void Authenticator::addMovement(Movement* movement) {
    recordedPattern_.addMovement(movement);
}

void Authenticator::clearMovements() {
    recordedPattern_ = MovementPattern();
}
