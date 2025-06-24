import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

Rectangle {
    id: gestionEtudiants

    property int columnCount: 3
    property var columnWidths: [150, 100, 200] // Largeurs synchronisées
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
        anchors.top: myColumn.bottom

        z: 2
        Repeater {
            model: ["Nom", "Âge", "Ville"]
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
        //anchors.fill: parent
        anchors.top: header.bottom

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // Définir les colonnes
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: etudiantsModel

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
                text: {
                    switch (column) {
                      case 0: return model.nom
                      case 1: return model.prenom
                      case 2: return model.mail
                      default: return ""
                  }
                }
                font.pixelSize: 14
            }
        }





        // Synchroniser le défilement avec l'en-tête
        //onContentXChanged: header.x = -contentX
    }

    // }
}
