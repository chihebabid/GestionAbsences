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

    color: "transparent"
    border.color: "transparent"
    border.width: 0

    Text {
        id: textItem
        anchors.centerIn: parent
        color:  "#0078d4"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
    }

    focus: true
}
