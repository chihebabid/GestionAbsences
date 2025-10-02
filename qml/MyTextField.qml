import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

TextField {
    placeholderTextColor: "#888"
    font.pixelSize: 14
    leftPadding: 12
    rightPadding: 12
    topPadding: 9
    bottomPadding: 9
    background: Rectangle {
        color: "#ffffff"
        radius: 5
        border.color: parent.enabled ? "#0078d4" : "#d0d0d0"
        border.width: parent.activeFocus ? 3 : 2
    }
}



