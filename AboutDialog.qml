import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: aboutDialog
    title: "À propos de Gestion des Absences"
    modal: true
    //standardButtons: Dialog.Ok
    anchors.centerIn: parent
    width: 400
    height: 300
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // Reset state to allow reopening
        onClosed: {
            visible = false
            focus = false
            parent = Overlay.overlay
            console.log("AboutDialog closed, state reset")
        }
        onOpened: {
            console.log("AboutDialog opened")
            forceActiveFocus()
        }



    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Application Icon or Logo (optional)
        Image {
            source: "qrc:/icons/enhance.png" // Replace with your icon path
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 80
            Layout.preferredHeight: 64
            visible: source !== ""
        }

        // Application Name
        Label {
            text: "Gestion des Absences"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // Version (optional, can be customized)
        Label {
            text: "Version 1.0.0"
            font.pixelSize: 14
            Layout.alignment: Qt.AlignHCenter
        }

        // Project Details
        Label {
            text: "Projet open-source sous licence LGPL3"
            font.pixelSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        // Company
        Label {
            text: "Entreprise : EnhanceTech"
            font.pixelSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        // Developer
        Label {
            text: "Développeur : Chiheb Ameur ABID"
            font.pixelSize: 12
            Layout.alignment: Qt.AlignHCenter
        }

        // Spacer to push content to the center
        Item {
            Layout.fillHeight: true
        }
    }

    // Customize dialog appearance
    background: Rectangle {
           color: root.palette.window // Reference the main window's palette
           border.color: palette.mid
           border.width: 1
           radius: 5
       }
    footer: DialogButtonBox {
            Button {
                text: qsTr("OK")
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                onClicked: {
                    //aboutDialog.close()
                                    aboutDialogLoader.active = false
                }
            }
        }

}
