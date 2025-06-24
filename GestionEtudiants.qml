import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Rectangle {
    id: gestionEtudiants

    //property int columnCount: 4
    property var columnWidths: [100,150, 150, 200] // Largeurs synchronisées
    Column {
        id: myColumn
        spacing: 20
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        Row {
            spacing: 10
            Text {
                text: "Section :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            ComboBox {
                id: sectionCombo
                textRole: "name"
                width: root.width * 0.3
                model: sectionModel
                onCurrentIndexChanged: {
                    if (sectionCombo.currentIndex >= 0) {
                        let sectionIndex = sectionCombo.currentIndex
                        let sectionId = sectionModel.getSectionId(sectionIndex)
                        tableEtudiants.model.loadEtudiantsForSection(sectionId)
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
            model: ["N˚ inscription","Nom", "Prénom", "E-mail"]
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
    TableView {
        id: tableEtudiants
        interactive: false
        leftMargin: 20
        anchors.top: header.bottom

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // Définir les colonnes
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: etudiantsModel
        height: 20
        ScrollBar.vertical: ScrollBar {
               policy: ScrollBar.AsNeeded
        }
       columnWidthProvider: function(col) { return columnWidths[col] }

        // Définir les en-têtes de colonnes

        delegate:
            Rectangle {
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
                      case 0: return model.inscri
                      case 1: return model.nom
                      case 2: return model.prenom
                      case 3: return model.mail
                      default: return ""
                  }
                }
                font.pixelSize: 14
            }
        }
    }
    Button {
        id: bSupprimer
        text: "Supprimer"
        anchors.margins: 10
        anchors.left: header.right
        anchors.top: header.top
        onClicked: {
            // moduleModel.deleteSelected()
        }
    }
    Button {
        text: "Importer"
        anchors.margins: 10
        anchors.left: header.right
        anchors.top: bSupprimer.bottom
        onClicked: {
            fileDialog.open()
        }
    }
    FileDialog {
            id: fileDialog
            title: "Choisir un fichier"
            nameFilters: ["Fichiers csv (*.csv)", "Tous les fichiers (*)"]
            onAccepted: {
                console.log("Fichier sélectionné :", fileDialog.file)
            }
            onRejected: {
                console.log("Sélection annulée")
            }
    }
}
