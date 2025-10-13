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
            RowLayout {
                anchors.fill: parent
                implicitWidth: parent.width
                anchors.margins: 0
                spacing: 6

                ColumnLayout {
                    id: myColumnEtudiants
                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width * 0.85 // 85% pour la colonne gauche
                    Layout.fillHeight: true // Remplit la hauteur
                    anchors.margins: 0
                    spacing: 1
                    property var columnWidths: [myColumnEtudiants.width
                        * .2, myColumnEtudiants.width * .3, myColumnEtudiants.width
                        * .3, myColumnEtudiants.width * .2 - 4]
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
                            onActivated: {
                                if (sectionCombo.currentIndex >= 0) {
                                    bImporter.enabled = true
                                    let sectionIndex = sectionCombo.currentIndex
                                    console.log("Section id: " + sectionIndex)
                                    let sectionId = sectionModel.getSectionId(
                                            sectionIndex)
                                    studentManager.fetchForSection(sectionId)
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true // Absorbe tout l'espace restant en bas
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
                        spacing: 1

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

                    MyTableView {
                        id: tableEtudiants
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        width: myColumnEtudiants.width
                        height: 300
                        property bool toRevert: false
                        model: studentManager.mModel
                        leftMargin: 0

                        interactive: false
                        delegate: Rectangle {
                            implicitWidth: -1
                            implicitHeight: 40
                            border.color: "#d0d0d0"
                            border.width: 0
                            color: model.selected ? "#87CEFA" : Qt.color(
                                                        "white")

                            required property int row
                            Rectangle {
                                width: parent.width
                                height: parent.height
                                anchors.left: parent.left
                                border.color: "#d0d0d0"
                                border.width: 1
                                color: "transparent"
                                TextField {
                                    id: sectionField
                                    background: null
                                    anchors.fill: parent
                                    anchors.margins: 0
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
                                    selectByMouse: true
                                    focus: false

                                    onFocusChanged: {
                                        if (focus) {
                                            mouseArea.enabled = false
                                        }
                                    }
                                    Keys.onEscapePressed: {
                                        focus = false
                                        mouseSection.enabled = true
                                    }
                                    onEditingFinished: {
                                        let id_student=studentManager.mModel.getStudentId(row)
                                        studentManager.updateStudent(id_student,column, sectionField.text)
                                        sectionField.deselect()
                                        sectionField.focus = false
                                        mouseSection.focus = true
                                        mouseSection.enabled = true
                                    }
                                }
                            }

                            MouseArea {
                                id: mouseSection
                                anchors.fill: parent
                                propagateComposedEvents: true
                                hoverEnabled: false
                                onClicked: mouse => {
                                               if (mouse.modifiers & Qt.ControlModifier) {
                                                   model.selected = !model.selected
                                               } else {
                                                   //studentManager.mModel.clearSelection()
                                                   studentManager.mModel.clearSelection()
                                                   mouseSection.enabled = false
                                                   sectionField.focus = true
                                                   sectionField.selectAll()
                                                   sectionField.forceActiveFocus()
                                               }
                                           }
                            }
                        }
                    }
                }
                Column {
                    spacing: 4
                    Layout.fillWidth: true // Prend le reste de la largeur (15%)
                    Layout.fillHeight: true // Remplit la hauteur pour éviter le centrage
                    Item {
                        height: 80
                        width: 1
                    }

                    MyButton {
                        id: bSupprimer
                        text: "Supprimer"
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
            }
            FileDialog {
                id: fileDialog
                title: "Choisir un fichier"
                nameFilters: ["Fichiers csv (*.csv)", "Tous les fichiers (*)"]
                onAccepted: {
                    console.log("Fichier sélectionné :", selectedFile)
                    let sectionIndex = sectionCombo.currentIndex
                    let sectionId = sectionModel.getSectionId(sectionIndex)
                    studentManager.importCSV(selectedFile, sectionId)
                }
                onRejected: {
                    console.log("Sélection annulée")
                }
            }
        }
    }
}
