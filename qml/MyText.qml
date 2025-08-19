import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    id: myTextRoot
    implicitWidth: textItem.implicitWidth + leftPadding + rightPadding
    implicitHeight: textItem.implicitHeight + topPadding + bottomPadding
    property alias text: textItem.text
    property alias font: textItem.font
    property bool enabled: true
    property real leftPadding: 12
    property real rightPadding: 12
    property real topPadding: 9
    property real bottomPadding: 9

    color: mouseArea.containsMouse && enabled ? "#0078d4" : "#ffffff"

    border.color: activeFocus && enabled ? "#0078d4" : (enabled ? "#0078d4" : "#c0c0c0")
    border.width: 0

    // Animation for color change
    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    // Click effect: scale change
    scale: mouseArea.pressed ? 0.90 : 1.0
    Behavior on scale {
        NumberAnimation {
            duration: 100
        }
    }

    Text {
        id: textItem
        anchors.centerIn: parent
        color: mouseArea.containsMouse && parent.enabled ? "#ffffff" : (parent.enabled ? "#0078d4" : "#c0c0c0")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onClicked: function (mouse) {
            myTextRoot.forceActiveFocus()
            mouse.accepted = false
        }
    }

    // Support keyboard focus
    focus: true
}
