import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

Rectangle {
    id: gestionEtudiants
    property int columnCount: 3
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
                        let semestre = semestreCombo.model[semestreCombo.currentIndex]
                        moduleModel.loadModulesForSection(sectionId, semestre)
                    }
                }
            }
        }
    }

    // Entêtes
    // Row {

    // Modèle de données (ListModel)
    TableModel {
            id: tableModel
            TableModelColumn { display: "name" }
            TableModelColumn { display: "color" }
            TableModelColumn { display: "Ok" }

            rows: [
                {
                    "name": "cat",
                    "color": "black",
                    "Ok": "1"
                },
                {
                    "name": "dog",
                    "color": "brown",
                    "Ok": "1"
                },
                {
                    "name": "bird",
                    "color": "white",
                    "Ok": "1"
                }
            ]
        }



    TableView {

        //anchors.fill: parent
        anchors.top: myColumn.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        // Définir les colonnes
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: tableModel

        columnWidthProvider: function(column) {
                      return 150 // Largeur fixe pour chaque colonne
                  }

        // Définir les en-têtes de colonnes

        delegate:
            Rectangle {
            implicitWidth: 150
            implicitHeight: 30
            border.color: "#ccc"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: display
                font.pixelSize: 14
            }
        }



        // En-tête personnalisée
        Row {
            id: header
            y: -parent.contentY - 30
            z: 2
            Repeater {
                model: ["Nom", "Âge", "Ville"]
                Rectangle {
                    width: 150
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

        // Synchroniser le défilement avec l'en-tête
        onContentXChanged: header.x = -contentX
    }

    // }
}
