import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import GestionAbsences
import "qrc:/qml/."

Rectangle {
    id: gestionEtudiants
    color: "transparent"
    property real tableWidth: root.width * .8
    property var columnWidths: [tableWidth * .2, tableWidth * .3, tableWidth
        * .3, tableWidth * .2] // Largeurs synchronisées

    MyGroupBox {
        width: parent.width * .8
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Gérer les étudiants")
        Column {
            id: myColumn
            spacing: 20
            anchors.top: gestionEtudiants.top
            anchors.topMargin: 80
            anchors.left: gestionEtudiants.left
            anchors.leftMargin: 20
            Row {
                spacing: 10
                MyText {
                    text: "Section :"
                }

                MyComboBox {
                    id: sectionCombo
                    textRole: "name"
                    width: root.width * 0.3
                    model: sectionModel
                    onCurrentIndexChanged: {
                        if (sectionCombo.currentIndex >= 0) {
                            bImporter.enabled = true
                            let sectionIndex = sectionCombo.currentIndex
                            let sectionId = sectionModel.getSectionId(
                                    sectionIndex)
                            tableEtudiants.model.loadEtudiantsForSection(
                                        sectionId)
                        }
                    }
                }
            }
        }
        // En-tête personnalisée
        Row {
            id: header
            anchors.topMargin: 30
            spacing: 1
            anchors.top: myColumn.bottom
            z: 2
            Rectangle {
                width: 19
                height: 30
                color: "transparent"
            }
            Repeater {
                model: ["N˚ inscription", "Nom", "Prénom", "E-mail"]
                Rectangle {
                    width: gestionEtudiants.columnWidths[index]
                    height: 30
                    color: "#f0f0f0"
                    border.color: "#ccc"
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.bold: true
                        font.pixelSize: 14
                    }
                }
            }
        }

        EtudiantsModel {
            id: etudiantsModel
        }

        MyTableView {
            id: tableEtudiants

            anchors.top: header.bottom
            height: 300
            anchors.left: parent.left
            //anchors.right: parent.right
            width: header.width
            anchors.bottom: parent.bottom


            model: etudiantsModel
            // Définir les en-têtes de colonnes
            delegate: Rectangle {
                implicitWidth: 150
                implicitHeight: 30
                border.color: "#ccc"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    horizontalAlignment: Text.AlignLeft
                    width: parent.width - 12
                    elide: Text.ElideLeft
                    text: {
                        switch (column) {
                        case 0:
                            return model.inscri
                        case 1:
                            return model.nom
                        case 2:
                            return model.prenom
                        case 3:
                            return model.mail
                        default:
                            return ""
                        }
                    }
                    font.pixelSize: 14
                }
            }
        }

        MyButton {
            id: bSupprimer
            text: "Supprimer"
            anchors.margins: 15
            anchors.left: header.right
            anchors.top: header.top
            onClicked: {

                // moduleModel.deleteSelected()
            }
        }
        MyButton {
            id: bImporter
            text: "Importer"
            anchors.margins: 15
            anchors.left: header.right
            anchors.top: bSupprimer.bottom
            enabled: false
            onClicked: {
                fileDialog.open()
            }
        }
        FileDialog {
            id: fileDialog
            title: "Choisir un fichier"
            nameFilters: ["Fichiers csv (*.csv)", "Tous les fichiers (*)"]
            onAccepted: {
                console.log("Fichier sélectionné :", selectedFile)
                let sectionIndex = sectionCombo.currentIndex
                let sectionId = sectionModel.getSectionId(sectionIndex)
                tableEtudiants.model.importCSV(selectedFile, sectionId)
            }
            onRejected: {
                console.log("Sélection annulée")
            }
        }
    }
}
