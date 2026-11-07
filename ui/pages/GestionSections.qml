import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import "qrc:/qml/."
import "../components"

Rectangle {
    id: gestionSections
    visible: true
    color: "transparent"

    ColumnLayout {
        id: pageLayout
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // En-tête : année universitaire
        MyText {
            Layout.alignment: Qt.AlignRight
            text: "Année universitaire : " + (databaseManager.educationYear)[0]+" - "+(databaseManager.educationYear)[1]
            font.pixelSize: 18
        }

        MyGroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("Gérer les sections")

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                // Ajout et suppression
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    MyTextField {
                        id: newSection
                        Layout.preferredWidth: 250
                        placeholderText: "Section"
                        onAccepted: btnAjouterSection.clicked()
                    }

                    MyButton {
                        id: btnAjouterSection
                        text: "Ajouter une section"
                        onClicked: {
                            if (newSection.text !== "") {
                                sectionModel.insertSection(newSection.text)
                                newSection.text = ""
                            }
                        }
                    }

                    // Pousse le bouton Supprimer vers la droite
                    Item {
                        Layout.fillWidth: true
                    }

                    MyButton {
                        text: "Supprimer"
                        onClicked: {
                            sectionModel.deleteSelected()
                        }
                    }
                }

                // Aide à l'utilisation de la liste
                Text {
                    text: "Clic : modifier le nom · Ctrl + clic : sélectionner pour suppression"
                    font.pixelSize: 12
                    color: "#555555"
                }

                // Liste des sections
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    interactive: false
                    model: sectionModel
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }
                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 40
                        border.color: "gray"
                        color: model.selected ? "#87CEFA" : "white" // Fond bleu si sélectionné
                        TextField {
                            id: sectionField
                            background: null
                            anchors.fill: parent
                            z: 0
                            anchors.margins: 2
                            text: valeur
                            focus: false
                            onEditingFinished: {
                                sectionModel.updateSection(index, text)
                                sectionField.z = 0
                                mouseSection.z = 1
                                sectionField.focus = false
                                sectionField.deselect()
                                mouseSection.focus = true
                            }
                        }

                        MouseArea {
                            id: mouseSection
                            anchors.fill: parent
                            z: 1
                            propagateComposedEvents: true
                            onClicked: function (mouse) {
                                if (mouse.modifiers & Qt.ControlModifier) {
                                    model.selected = !model.selected
                                } else {
                                    sectionModel.clearSelection()
                                    sectionField.forceActiveFocus()
                                    // sectionField.selectAll()
                                    mouseSection.focus = false
                                    sectionField.z = 1
                                    mouseSection.z = 0
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

