import QtQuick
import QtQuick.Controls

GroupBox {
    anchors.margins: 5
    label: Label {
        text: parent.title
        color: "#0078d4"
        font.pixelSize: 14
        font.bold: true
        padding: 10
    }
    background: Rectangle {
        border.color: "#0078d4" // ← Couleur de la bordure
        border.width: 2
        radius: 5
        color: "transparent"
    }
}

