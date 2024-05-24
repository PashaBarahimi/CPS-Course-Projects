import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import MotionBasedAuthenticator

Page {
    id: page
    visible: false

    property string actionName: ""
    property list<Movement> movements: []

    property var sendSuccessNotification: notification.success
    property var sendFailureNotification: notification.failure

    signal recordingStart()
    signal recordingEnd()
    signal backClicked()

    property string currentAngle: "0°"

    function appendMovement(movement) {
        movements.push(movement);
        onMovementsChanged();
    }

    function updateLastMovement(movement) {
        if (movements.length > 0) {
            movements[movements.length - 1] = movement;
            onMovementsChanged();
        }
    }

    function popMovement() {
        if (movements.length > 0) {
            movements.pop();
            onMovementsChanged();
        }
    }

    function clearMovements() {
        movements = [];
        onMovementsChanged();
    }

    function prepareToClose() {
        recordButton.stopIfStarted();
    }

    function changeAngle(angleStr) {
        currentAngle = angleStr;
    }

    onMovementsChanged: {
        canvas.requestPaint();
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

        RowLayout {
            id: angleDisplay
            anchors.left: parent.left
            anchors.right: parent.right
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: "Angle: "
                font.pixelSize: 20
            }

            Label {
                text: page.currentAngle
                font.pixelSize: 20
                color: "blue"
            }
        }

        Notification {
            id: notification
        }

        Rectangle {
            id: diagram
            Layout.preferredWidth: parent.width * 0.9
            Layout.preferredHeight: parent.height * 0.4
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            radius: 10
            color: "White"

            Canvas {
                id: canvas
                width: parent.width * 0.95
                height: parent.height * 0.95
                anchors.centerIn: parent

                function arrow(context, from, to) {
                    const dx = to.x - from.x;
                    const dy = to.y - from.y;
                    const headlen = 15;
                    const angle = Math.atan2(dy, dx);

                    context.beginPath();
                    context.moveTo(from.x, from.y);
                    context.lineTo(to.x, to.y);
                    context.stroke();
                    context.beginPath();
                    context.moveTo(to.x - headlen * Math.cos(angle - Math.PI / 6), to.y - headlen * Math.sin(angle - Math.PI / 6));
                    context.lineTo(to.x, to.y);
                    context.lineTo(to.x - headlen * Math.cos(angle + Math.PI / 6), to.y - headlen * Math.sin(angle + Math.PI / 6));
                    context.stroke();
                }

                onPaint: {
                    var ctx = getContext("2d");
                    ctx.fillStyle = "White"
                    ctx.fillRect(0, 0, width, height);

                    ctx.lineWidth = 2;

                    var bounds = getPosBounds();
                    var scaledMovements = movements.map(function(movement) {
                        return scaleMovement(bounds, movement);
                    });

                    for (var i = 0; i < scaledMovements.length; i++) {
                        if (i === swipeView.currentIndex) {
                            ctx.strokeStyle = "Red";
                        } else {
                            ctx.strokeStyle = "Blue";
                        }
                        var movement = scaledMovements[i];
                        arrow(ctx, movement.from, movement.to);
                    }
                }

                function getPosBounds() {
                    var minX = 0;
                    var maxX = 0;
                    var minY = 0;
                    var maxY = 0;

                    for (var i = 0; i < movements.length; i++) {
                        minX = Math.min(minX, movements[i].start.x);
                        maxX = Math.max(maxX, movements[i].start.x);
                        minY = Math.min(minY, movements[i].start.y);
                        maxY = Math.max(maxY, movements[i].start.y);

                        minX = Math.min(minX, movements[i].end.x);
                        maxX = Math.max(maxX, movements[i].end.x);
                        minY = Math.min(minY, movements[i].end.y);
                        maxY = Math.max(maxY, movements[i].end.y);
                    }


                    return {
                        minX: minX,
                        maxX: maxX,
                        minY: minY,
                        maxY: maxY,
                    }
                }

                function scaleMovement(bounds, movement) {
                    var rangeX = bounds.maxX === bounds.minX ? 1 : bounds.maxX - bounds.minX;
                    var rangeY = bounds.maxY === bounds.minY ? 1 : bounds.maxY - bounds.minY;
                    var paddingX = width * 0.05;
                    var paddingY = height * 0.05;

                    return {
                        from: {
                            x: ((movement.start.x - bounds.minX) / rangeX * (width * 0.90)) + paddingX,
                            y: ((movement.start.y - bounds.minY) / rangeY * (height * 0.90)) + paddingY,
                        },
                        to: {
                            x: ((movement.end.x - bounds.minX) / rangeX * (width * 0.90)) + paddingX,
                            y: ((movement.end.y - bounds.minY) / rangeY * (height * 0.90)) + paddingY,
                        },
                    }
                }
            }
        }

        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.3
            Layout.alignment: Qt.AlignCenter
            currentIndex: 0

            property bool isSwiping: false

            Repeater {
                model: movements
                delegate: Item {
                    Rectangle {
                        color: swipeView.isSwiping ? "Grey" : "Transparent"
                        width: swipeView.width
                        height: swipeView.height

                        MouseArea {
                            anchors.fill: parent

                            onPressedChanged: {
                                swipeView.isSwiping = !swipeView.isSwiping
                            }
                        }

                        Label {
                            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                            font.pixelSize: swipeView.height * 0.07
                            padding: 10
                            color: "White"
                            text: modelData.toStr(index + 1, page.movements.length)
                        }
                    }
                }
            }

            onCountChanged: {
                if (swipeView.count === movements.length) {
                    swipeView.currentIndex = swipeView.count - 1;
                }
            }

            onCurrentIndexChanged: {
                canvas.requestPaint();
            }
        }

        PageIndicator {
            count: swipeView.count
            currentIndex: swipeView.currentIndex
            anchors.top: swipeView.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }

        RowLayout {
            id: footer
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.08

            Button {
                id: recordButton
                Layout.alignment: Qt.AlignCenter
                text: isStarted ? "Stop " + actionName : "Start " + actionName

                property bool isStarted: false

                onClicked: {
                    isStarted = !isStarted
                    if (isStarted) {
                        recordingStart();
                    } else {
                        recordingEnd();
                    }
                }

                function stopIfStarted() {
                    if (isStarted) {
                        isStarted = false;
                        recordingEnd();
                    }
                }
            }
        }
    }
}
