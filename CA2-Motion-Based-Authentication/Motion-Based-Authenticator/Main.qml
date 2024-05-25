import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import MotionBasedAuthenticator

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Authenticator")

    Material.theme: Material.Dark
    Material.accent: Material.Teal
    Material.primary: Material.BlueGrey

    onClosing: {
        if (stackView.depth > 1) {
            stackView.currentItem.prepareToClose()
            stackView.pop()
            close.accepted = false
        }
    }

    Backend {
        id: backend

        onCalibrationFinished: (success) => {
            sensorsPage.calibrationFinished(success)
        }

        onUpdateGyroData: (x, y, z, xCal, yCal, zCal) => {
            sensorsPage.updateGyroscopeValues(x, y, z, xCal, yCal, zCal)
        }

        onUpdateAccelData: (x, y, z, xCal, yCal, zCal) => {
            sensorsPage.updateAccelerometerValues(x, y, z, xCal, yCal, zCal)
        }

        onPatternRecordingAddMovement: (movement) => {
            recorderPage.appendMovement(movement)
        }

        onPatternRecordingUpdateMovement: (movement) => {
            recorderPage.updateLastMovement(movement)
        }

        onPatternRecordingPopMovement: {
            recorderPage.popMovement()
        }

        onPatternRecordingClearMovements: {
            recorderPage.clearMovements()
        }

        onPatternRecordingSuccessful: {
            recorderPage.sendSuccessNotification("Pattern Recorded Successfully!")
        }

        onPatternRecordingFailed: (error) => {
            recorderPage.sendFailureNotification("Pattern Recording Failed! " + error)
        }

        onPatternRecordingChangeAngle: (angleStr) => {
            recorderPage.changeAngle(angleStr)
        }

        onAuthenticationAddMovement: (movement) => {
            authPage.appendMovement(movement)
        }

        onAuthenticationUpdateMovement: (movement) => {
            authPage.updateLastMovement(movement)
        }

        onAuthenticationPopMovement: {
            authPage.popMovement()
        }

        onAuthenticationClearMovements: {
            authPage.clearMovements()
        }

        onAuthenticationSuccessful: {
            authPage.sendSuccessNotification("Authentication Successful!")
        }

        onAuthenticationFailed: (error) => {
            authPage.sendFailureNotification("Authentication Failed! " + error)
        }

        onAuthenticationChangeAngle: (angleStr) => {
            authPage.changeAngle(angleStr)
        }
    }

    ColumnLayout {
        anchors.fill: parent

        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: mainPage
        }

        Page {
            id: mainPage
            title: "Motion-Based Authentication"

            Rectangle {
                width: parent.width
                height: 100
                color: "Transparent"

                Text {
                    anchors.centerIn: parent
                    text: "Motion-Based Authentication"
                    font.bold: true
                    font.pixelSize: 20
                    color: "White"
                }
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10

                Notification {
                    id: notification
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: mainPage.width * 0.9
                    Layout.preferredHeight: mainPage.height * 0.05
                }

                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Record a New Pattern"
                    onClicked: {
                        stackView.push(recorderPage)
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Authenticate"
                    onClicked: {
                        stackView.push(authPage)
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Sensors"
                    onClicked: {
                        stackView.push(sensorsPage)
                        backend.startShowingSensors()
                    }
                }
            }
        }

        PatternPage {
            id: recorderPage
            title: "Record a New Pattern"
            actionName: "Recording"

            onBackClicked: {
                stackView.pop()
            }

            onRecordingStart: {
                backend.startPatternRecording()
            }

            onRecordingEnd: {
                backend.stopPatternRecording()
            }
        }

        PatternPage {
            id: authPage
            title: "Authenticate"
            actionName: "Authentication"

            onBackClicked: {
                stackView.pop()
            }

            onRecordingStart: {
                backend.startAuthentication()
            }

            onRecordingEnd: {
                backend.stopAuthentication()
            }
        }

        SensorsPage {
            id: sensorsPage
            title: "Sensors"

            onBackClicked: {
                stackView.pop()
                backend.stopShowingSensors()
            }

            onCalibrationRequested: {
                backend.startCalibration()
            }

        }
    }
}
