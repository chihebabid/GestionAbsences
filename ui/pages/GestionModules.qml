import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import "qrc:/qml/."
import "../components"

Rectangle {
    id: moduleInterface
    color: "transparent"

    ModuleModel {
        id: moduleModel
    }

    // Recharger les modules selon la section et le semestre sélectionnés
    function updateListeModules() {
        if (sectionCombo.currentIndex >= 0 && semestreCombo.currentIndex >= 0) {
            let sectionId = sectionModel.getSectionId(sectionCombo.currentIndex)
            let semestre = semestreCombo.model[semestreCombo.currentIndex]
            moduleModel.loadModulesForSection(sectionId, semestre)
        }
    }

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

        // Sélection du semestre et de la section
        MyGroupBox {
            Layout.fillWidth: true
            title: qsTr("Sélectionner la section")

            RowLayout {
                width: parent.width
                spacing: 8

                MyText {
                    text: "Semestre :"
                }

                MyComboBox {
                    id: semestreCombo
                    Layout.preferredWidth: 100
                    model: [1, 2]
                    onCurrentIndexChanged: moduleInterface.updateListeModules()
                }

                Espacement {}

                MyText {
                    text: "Section :"
                }

                MyComboBox {
                    id: sectionCombo
                    Layout.fillWidth: true
                    textRole: "name"
                    model: sectionModel
                    onCurrentIndexChanged: moduleInterface.updateListeModules()
                }
            }
        }

        MyGroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("Gérer les modules")

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                // Ajout et suppression
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    MyTextField {
                        id: newModule
                        Layout.preferredWidth: 250
                        placeholderText: "Module"
                        onAccepted: btnAjouterModule.clicked()
                    }

                    MyButton {
                        id: btnAjouterModule
                        text: "Ajouter un module"
                        // Un module doit être rattaché à une section
                        enabled: sectionCombo.currentIndex >= 0
                        onClicked: {
                            if (newModule.text !== "") {
                                let index = sectionCombo.currentIndex
                                let sectionId = sectionModel.getSectionId(index)
                                moduleModel.addModuleForSection(
                                            newModule.text, sectionId,
                                            semestreCombo.currentValue)
                                newModule.text = ""
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
                            moduleModel.deleteSelected()
                        }
                    }
                }

                // Aide à l'utilisation de la liste
                Text {
                    text: "Clic : modifier le nom · Ctrl + clic : sélectionner pour suppression"
                    font.pixelSize: 12
                    color: "#555555"
                }

                // Liste affichant la liste des modules
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    interactive: false
                    model: moduleModel
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }
                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 40
                        border.color: "gray"
                        color: model.selected ? "#87CEFA" : "white" // Fond bleu si sélectionné
                        TextField {
                            id: moduleField
                            background: null
                            anchors.fill: parent
                            z: 0
                            anchors.margins: 2
                            text: valeur
                            focus: false
                            onEditingFinished: {
                                moduleModel.updateModule(index, text)
                                moduleField.z = 0
                                mouseSection.z = 1
                                moduleField.focus = false
                                moduleField.deselect()
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
                                    moduleField.forceActiveFocus()
                                    mouseSection.focus = false
                                    moduleField.z = 1
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

