#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    patternRecognizer_ = new PatternRecognizer(accelerometerHandler_);

    connect(accelerometerHandler_, &AccelerometerHandler::calibrationFinished, this, &Backend::calibrationFinished);
}

void Backend::startCalibration()
{
    qDebug() << "Calibration started";
    accelerometerHandler_->startCalibrate();
}

void Backend::startPatternRecording()
{
    qDebug() << "Pattern recording started";
    patternRecognizer_->startRecording();
}

void Backend::stopPatternRecording()
{
    // This functions stops the recording mentioned in the above method
    // We must also save the complete pattern (a list of movements)
}

void Backend::startAuthentication()
{
    // same as startPatternRecording but called from authentication page
}

void Backend::stopAuthentication()
{
    // same as stopPatternRecording but we should also check the equality
    // of the authentication pattern with the recorded pattern
}

// Signals
//
// calibrationFinished(bool) is used to inform the UI that the calibration process is finished.
// Otherwise, the busy indicator will stay spinning
// it gets a boolean which shows if the process was successful. No idea if we ever may pass false to it
// for example we can check if the standard deviation of the values is not too much
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
