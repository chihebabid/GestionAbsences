import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

TextField {
    placeholderTextColor: "#888"
    font.pixelSize: 14
    background: Rectangle {
        color: "#ffffff"
        radius: 5
        border.color: parent.activeFocus ? "#0078d4" : "#d0d0d0"
        border.width: parent.activeFocus ? 2 : 1
    }
}



