import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: notification
    opacity: 0
    Layout.preferredWidth: parent.width * 0.9
    Layout.preferredHeight: parent.height * 0.05
    Layout.alignment: Qt.AlignCenter
    radius: 30
    color: "Transparent"

    NumberAnimation on opacity {
        id: notificationFadeIn
        from: 0
        to: 1
        duration: 200
    }

    NumberAnimation on opacity {
        id: notificationFadeOut
        from: 1
        to: 0
        duration: 200
    }

    Label {
        id: notificationLabel
        text: ""
        color: "White"
        anchors.centerIn: parent
    }

    Timer {
        id: timer
        interval: 5000
        running: false
        repeat: false
        onTriggered: {
            notificationFadeOut.start();
        }
    }

    function success(message) {
        notificationLabel.text = message;
        notification.color = "Green";
        notificationFadeIn.start();
        timer.start();
    }

    function failure(message) {
        notificationLabel.text = message;
        notification.color = "Red";
        notificationFadeIn.start();
        timer.start();
    }
}
