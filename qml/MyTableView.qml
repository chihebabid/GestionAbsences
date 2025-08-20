import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

TableView {
    clip: true
    interactive: true
    boundsBehavior: Flickable.StopAtBounds
    flickableDirection: Flickable.AutoFlickIfNeeded
    pressDelay: 999999
    leftMargin: 5
    rightMargin: 5
    columnSpacing: 1
    rowSpacing: 1
    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AsNeeded
    }
    columnWidthProvider: function (col) {
        return parent.columnWidths[col]
    }
}
