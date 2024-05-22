#include "patternrecognizer.h"
#include <QDebug>

PatternRecognizer::PatternRecognizer(AccelerometerHandler* accelerometerHandler, QObject *parent)
    : QObject(parent), isRecording(false) {
    accelerometerHandler_ = accelerometerHandler;
    // connect(sensor, &QAccelerometer::readingChanged, this, &PatternRecognizer::handleReading);
}

void PatternRecognizer::startRecording() {
    recordedPattern = MovementPattern();

}

// void PatternRecognizer::stopRecording() {
//     isRecording = false;
//     sensor->stop();
//     processReadings();
// }

MovementPattern PatternRecognizer::getRecordedPattern() const {
    return recordedPattern;
}

// bool PatternRecognizer::authenticateMovement(const MovementPattern& pattern) const {
//     return recordedPattern.matches(pattern);
// }

// void PatternRecognizer::handleReading() {
//     if (isRecording) {
//         readings.append(*sensor->reading());
//     }
// }

// void PatternRecognizer::processReadings() {
//     // Example processing logic: simplify the real data processing and conversion
//     // into PatternSegment objects based on the application requirements.
//     // For simplicity, assume we process readings into segments here.
//     // This is a placeholder implementation.

//     if (readings.size() < 2) {
//         return;
//     }

//     // Simplified example of processing readings into segments
//     for (int i = 1; i < readings.size(); ++i) {
//         auto prevReading = readings[i - 1];
//         auto currReading = readings[i];

//         // Calculate a simple direction and length between points
//         double dx = currReading.x() - prevReading.x();
//         double dy = currReading.y() - prevReading.y();
//         double length = std::sqrt(dx * dx + dy * dy);
//         double direction = std::atan2(dy, dx) * (180.0 / M_PI);

//         // Assuming a fixed orientation for simplicity (0, 90, 180, 270)
//         int orientation = 0;  // Example value; in real case, derive from sensor data

//         recordedPattern.addSegment(PatternSegment(length, direction, orientation));
//     }
// }
