import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ItemDelegate {
    enabled: !isSpacer(index) && !isSeparator(index)

    //
    // Alias to parent list view
    //
    property ListModel model
    property ListView pageSelector

    //
    // Détection des types d’éléments
    //
    function isSpacer(index) {
        if (typeof(model.get(index).spacer) !== "undefined")
            return model.get(index).spacer
        return false
    }

    function isLink(index) {
        if (typeof(model.get(index).link) !== "undefined")
            return model.get(index).link
        return false
    }

    function isSeparator(index) {
        if (typeof(model.get(index).separator) !== "undefined")
            return model.get(index).separator
        return false
    }

    function iconSource(index) {
        if (typeof(model.get(index).pageIcon) !== "undefined")
            return model.get(index).pageIcon
        return ""
    }

    function itemText(index) {
        if (typeof(model.get(index).pageTitle) !== "undefined")
            return model.get(index).pageTitle
        return ""
    }

    function hasSeparatorText(index) {
        return isSeparator(index) && typeof(model.get(index).separatorText) !== "undefined"
    }

    //
    // Mise en surbrillance
    //
    highlighted: ListView.isCurrentItem ? !isLink(index) : false

    // Gestion du survol avec la couleur de surbrillance du système
    hoverEnabled: true
    background: Rectangle {
        color: parent.hovered && !isSeparator(index) && !isSpacer(index) ? palette.highlight : "#ffffff"
        Behavior on color { ColorAnimation { duration: 200 } } // Transition fluide
    }


    height: 48

    //
    // Layout pour séparateur
    //
    ColumnLayout {
        spacing: 8
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: isSeparator(index)

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            height: 0.5
            opacity: 0.20
            color: "#000000"
            Layout.fillWidth: true
        }

        Label {
            opacity: 0.54
            color: "#000000"
            font.pixelSize: 14
            font.weight: Font.Medium
            text: hasSeparatorText(index) ? model.get(index).separatorText : ""
            Layout.margins: 16
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
        }
    }

    //
    // Layout normal (icône + texte)
    //
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
        visible: !isSpacer(index)

        //anchors.margins: 16

        RowLayout {            
            anchors.fill: parent

            Image {
                smooth: true
                opacity: parent.parent.hovered ? 1.0 : 0.54
                fillMode: Image.PreserveAspectFit
                source: iconSource(index)
                sourceSize: Qt.size(48, 48)
                Layout.alignment: Qt.AlignVCenter

            }

            Item {
                width: 12
                Layout.alignment: Qt.AlignVCenter
            }

            Label {
                opacity: parent.parent.hovered ? 1.0 : 0.87
                font.pixelSize: 14
                font.weight: Font.Medium
                text: itemText(index)
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
