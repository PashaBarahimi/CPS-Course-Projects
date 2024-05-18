import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import MotionBasedAuthenticator

Page {
    id: page
    visible: false

    property var sendSuccessNotification: notification.success
    property var sendFailureNotification: notification.failure
    property var calibrationFinished: calibrateButton.done

    signal calibrationRequested()
    signal backClicked()

    function updateGyroscopeValues(x, y, z, xCal, yCal, zCal) {
        var xValue = x.toFixed(2);
        var yValue = y.toFixed(2);
        var zValue = z.toFixed(2);
        gyroValues.updateValues(xValue, yValue, zValue);

        var xCalValue = xCal.toFixed(2);
        var yCalValue = yCal.toFixed(2);
        var zCalValue = zCal.toFixed(2);
        gyroValues.updateCalibratedValues(xCalValue, yCalValue, zCalValue);
    }

    function updateAccelerometerValues(x, y, z, xCal, yCal, zCal) {
        var xValue = x.toFixed(2);
        var yValue = y.toFixed(2);
        var zValue = z.toFixed(2);
        accelValues.updateValues(xValue, yValue, zValue);

        var xCalValue = xCal.toFixed(2);
        var yCalValue = yCal.toFixed(2);
        var zCalValue = zCal.toFixed(2);
        accelValues.updateCalibratedValues(xCalValue, yCalValue, zCalValue);
    }

    function prepareToClose() {
        calibrateButton.done(false);
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        RowLayout {
            id: header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            Button {
                id: backButton
                text: "Back"
                onClicked: {
                    page.prepareToClose();
                    backClicked();
                }
                anchors.left: parent.left
            }

            Label {
                text: page.title
                anchors.centerIn: parent
            }
        }

        Notification {
            id: notification
        }

        ColumnLayout {
            id: sensorValues
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.6
            spacing: 10

            SensorValues {
                id: gyroValues
                title: "Gyroscope"
                unit: "rad/s"
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: "White"
            }

            SensorValues {
                id: accelValues
                title: "Accelerometer"
                unit: "m/s²"
                Layout.fillWidth: true
                Layout.preferredHeight: parent.height * 0.5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            }
        }

        BusyIndicator {
            id: busyIndicator
            Layout.alignment: Qt.AlignCenter
            running: false
        }

        Label {
            text: "Please hold your device steady and press the calibrate button!"
            Layout.alignment: Qt.AlignHCenter
            font.pointSize: 12
        }

        RowLayout {
            id: footer
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.08

            Button {
                id: calibrateButton
                Layout.alignment: Qt.AlignCenter
                text: "Calibrate"

                property bool isStarted: false

                onClicked: {
                    if (isStarted) {
                        return;
                    }
                    busyIndicator.running = true;
                    isStarted = true;
                    calibrationRequested();
                }

                function done(success) {
                    if (isStarted) {
                        isStarted = false;
                        busyIndicator.running = false;
                        if (success) {
                            sendSuccessNotification("Calibration Successful!");
                        } else {
                            sendFailureNotification("Calibration Failed!");
                        }
                    }
                }
            }
        }
    }
}
