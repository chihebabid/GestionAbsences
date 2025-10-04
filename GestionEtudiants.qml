import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import com.enhancetech.absences
import "qrc:/qml/."

Rectangle {
    id: gestionEtudiants
    width: parent.width
    height: parent.height
    color: "transparent"

    Column {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 5

        MyGroupBox {
            width: parent.width * .9
            height: parent.height * .9
            title: qsTr("Gérer les étudiants")
            ColumnLayout {
                id: myColumnEtudiants
                anchors.fill: parent
                implicitWidth: parent.width
                anchors.margins: 0
                spacing: 1
                property var columnWidths: [myColumnEtudiants.width * .2, myColumnEtudiants.width
                    * .3, myColumnEtudiants.width * .3, myColumnEtudiants.width * .2]
                Row {
                    spacing: 5
                    anchors.margins: 5
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
                                let sectionId = studentManager.fetchForSection(
                                        sectionIndex)
                                /*tableEtudiants.model.loadEtudiantsForSection(
                                            sectionId)*/
                            }
                        }
                    }
                }

                Item {
                    height: 6
                }

                // Tableau
                Row {
                    id: header
                    Layout.fillWidth: true
                    width: myColumnEtudiants.width
                    height: 40
                    spacing: 0

                    Repeater {
                        model: ["N˚ inscription", "Nom", "Prénom", "E-mail"]
                        Rectangle {
                            width: myColumnEtudiants.columnWidths[index]
                            height: parent.height
                            color: "#0078d4"
                            border.color: "#d0d0d0"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.pixelSize: 14
                                font.bold: true
                                color: "#ffffff"
                            }
                        }
                    }
                }

                EtudiantsModel {
                    id: etudiantsModel
                }

                MyTableView {
                    id: tableEtudiants
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    width: myColumnEtudiants.width
                    height: 300
                    property bool toRevert: false
                    model: studentManager.mModel

                    delegate: Rectangle {
                        implicitHeight: 40
                        border.color: "#d0d0d0"
                        border.width: 0
                        color: "#ffffff"
                        required property int row
                        Rectangle {
                            width: parent.width
                            height: parent.height
                            anchors.left: parent.left
                            border.width: 1
                            border.color: "#d0d0d0"
                            color: "transparent"
                        }

                        Item {
                            anchors.fill: parent
                            Loader {
                                anchors.fill: parent
                                sourceComponent: orgTextItem
                            }

                            Component {
                                id: orgTextItem
                                Item {
                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 0
                                        anchors.verticalCenter: parent.verticalCenter
                                        leftPadding: 4
                                        rightPadding: 4
                                        font.pixelSize: 14
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
                                    }
                                }
                            }
                        }
                    }
                }

                MyButton {
                    id: bSupprimer
                    text: "Supprimer"
                    anchors.margins: 15
                    onClicked: {

                        // moduleModel.deleteSelected()
                    }
                }
                MyButton {
                    id: bImporter
                    text: "Importer"
                    enabled: false
                    onClicked: {
                        fileDialog.open()
                    }
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
}
