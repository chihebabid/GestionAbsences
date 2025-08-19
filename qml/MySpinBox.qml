import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

SpinBox {
    id: control
    font.pixelSize: 14
    leftPadding: 12
    rightPadding: 20
    topPadding: 10
    bottomPadding: 10
    spacing: 2

    // Background
    background: Rectangle {
        id: spinBackground
        color: control.hovered && control.enabled ? "#0078d4" : "#ffffff"
        radius: 5
        border.color: control.activeFocus && control.enabled ? "#0078d4" : (control.enabled ? "#0078d4" : "#c0c0c0")
        border.width: control.activeFocus ? 3 : 2

        Behavior on color { ColorAnimation { duration: 200 } }
        scale: control.pressed ? 0.90 : 1.0
        Behavior on scale { NumberAnimation { duration: 100 } }
    }

    // Text input
    contentItem: TextInput {
        text: control.textFromValue(control.value, control.locale)
        color: control.hovered && control.enabled ? "#ffffff" : (control.enabled ? "#0078d4" : "#c0c0c0")
        font: control.font
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        leftPadding: 12
        rightPadding: 30
        selectByMouse: true
        validator: control.validator

        onTextEdited: {
            var newValue = control.valueFromText(text, control.locale)
            if (!isNaN(newValue)) control.value = newValue
        }
    }

    // Up indicator
    up.indicator: Item {
        width: 12
        height: 8
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.rightMargin: 4

        Canvas {
            id: upCanvas
            anchors.fill: parent
            contextType: "2d"
            onPaint: {
                context.reset()
                context.beginPath()
                context.moveTo(0, height)
                context.lineTo(width / 2, 0)
                context.lineTo(width, height)
                context.closePath()
                context.fillStyle = control.hovered && control.enabled ? "#ffffff" : (control.enabled ? "#0078d4" : "#c0c0c0")
                context.fill()
            }

            Connections {
                target: control
                function onHoveredChanged() { upCanvas.requestPaint() }
                function onEnabledChanged() { upCanvas.requestPaint() }
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            //enabled: control.up.enabled
            onClicked: {
                control.increase()
                console.log("MySpinBox: Up indicator clicked, value:", control.value)
            }
        }
    }

    // Down indicator
    down.indicator: Item {
        width: 12
        height: 8
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.rightMargin: 4

        Canvas {
            id: downCanvas
            anchors.fill: parent
            contextType: "2d"
            onPaint: {
                context.reset()
                context.beginPath()
                context.moveTo(0, 0)
                context.lineTo(width / 2, height)
                context.lineTo(width, 0)
                context.closePath()
                context.fillStyle = control.hovered && control.enabled ? "#ffffff" : (control.enabled ? "#0078d4" : "#c0c0c0")
                context.fill()
            }

            Connections {
                target: control
                function onHoveredChanged() { downCanvas.requestPaint() }
                function onEnabledChanged() { downCanvas.requestPaint() }
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            //enabled: control.down.enabled
            onClicked: {
                control.decrease()
                console.log("MySpinBox: Down indicator clicked, value:", control.value)
            }
        }
    }
}
