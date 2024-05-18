import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "Transparent"

    property string title: ""
    property string unit: ""

    function updateValues(xVal, yVal, zVal) {
        x.text = xVal
        y.text = yVal
        z.text = zVal
    }

    function updateCalibratedValues(xCalValue, yCalValue, zCalValue) {
        xCal.text = xCalValue
        yCal.text = yCalValue
        zCal.text = zCalValue
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            text: title
            font.bold: true
            font.pointSize: 22
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            leftPadding: 10
            text: "Actual Values"
            font.bold: true
            font.pointSize: 18
            Layout.alignment: Qt.AlignLeft
        }

        RowLayout {
            spacing: 10
            Label {
                leftPadding: 10
                text: "X:"
            }
            Label {
                id: x
                text: "0"
            }
            Label {
                text: unit
            }

            Label {
                text: "Y:"
            }
            Label {
                id: y
                text: "0"
            }
            Label {
                text: unit
            }

            Label {
                text: "Z:"
            }
            Label {
                id: z
                text: "0"
            }
            Label {
                text: unit
            }
        }

        Label {
            leftPadding: 10
            text: "Calibrated Values"
            font.bold: true
            font.pointSize: 18
            Layout.alignment: Qt.AlignLeft
        }

        RowLayout {
            spacing: 10
            Label {
                leftPadding: 10
                text: "X:"
            }
            Label {
                id: xCal
                text: "0"
            }
            Label {
                text: unit
            }

            Label {
                text: "Y:"
            }
            Label {
                id: yCal
                text: "0"
            }
            Label {
                text: unit
            }

            Label {
                text: "Z:"
            }
            Label {
                id: zCal
                text: "0"
            }
            Label {
                text: unit
            }
        }
    }
}
