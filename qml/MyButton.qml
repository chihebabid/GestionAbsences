import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Button {
    font.pixelSize: 14
    leftPadding: 12
    rightPadding: 12
    topPadding: 14
    bottomPadding: 14
    background: Rectangle {
        id: buttonBackground
        color: parent.hovered && parent.enabled ? "#0078d4" : "#ffffff"

        radius: 5
        border.color: parent.activeFocus && parent.enabled ? "#0078d4" : ( parent.enabled ? "#0078d4" : "#c0c0c0")
        border.width: parent.activeFocus ? 3 : 2

        // Animation pour le clic
        Behavior on color {
            ColorAnimation {
                duration: 200
            }
        }

        // Effet de clic : changement d'échelle
        scale: parent.pressed ? 0.90 : 1.0
        Behavior on scale {
            NumberAnimation {
                duration: 100
            }
        }
    }

    contentItem: Text {
        text: parent.text
        color: parent.hovered && parent.enabled ? "#ffffff" : (parent.enabled ? "#0078d4" : "#c0c0c0")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font: parent.font
    }
}
