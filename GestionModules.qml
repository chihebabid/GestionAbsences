import QtQuick
import QtQuick.Controls
import "qrc:/qml/."
Rectangle {
    id: moduleInterface
    color: "transparent"
    ModuleModel {
        id: moduleModel
    }

    Column {
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

            Item {
                   width: 40
                   height: 1
            }

            Text {
                text: "Semestre :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            ComboBox {
                id: semestreCombo
                model: [1, 2]
                onCurrentIndexChanged: {
                    if (currentIndex >= 0) {
                        let sectionIndex = sectionCombo.currentIndex
                        let sectionId = sectionModel.getSectionId(sectionIndex)
                        let semestre = semestreCombo.model[semestreCombo.currentIndex]
                        moduleModel.loadModulesForSection(sectionId, semestre)
                    }
                }
            }
        }

        Row {
            spacing: 10

            TextField {
                id: newModule
                placeholderText: "Module"
                width: 200
            }

            MyButton {
                text: "Ajouter un module"
                onClicked: {
                    if (newModule.text !== "") {
                        let index=sectionCombo.currentIndex
                        let sectionId = sectionModel.getSectionId(index)
                        moduleModel.addModuleForSection(newModule.text,sectionId,semestreCombo.currentValue)
                        newModule.text = ""
                    }
                }
            }
        }

        // Liste affichant la liste des modules
        Row {
            //width: root* .90
            spacing: 10
            ListView {
                width: root.width * 0.6
                height: 300
                interactive: false
                model: moduleModel
                ScrollBar.vertical: ScrollBar {
                       policy: ScrollBar.AsNeeded
                }
                delegate: Rectangle {
                    width: root.width * 0.6
                    height: 30
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
                            moduleField.z=0
                            mouseSection.z=1
                            moduleField.focus = false
                            moduleField.deselect()
                            mouseSection.focus=true;
                        }
                    }

                    MouseArea {
                        id: mouseSection
                        anchors.fill: parent
                        z: 1
                        propagateComposedEvents: true
                        onClicked: function(mouse) {
                           if (mouse.modifiers & Qt.ControlModifier) {
                                model.selected = !model.selected;
                            }
                           else {
                               sectionModel.clearSelection()
                               moduleField.forceActiveFocus()
                               mouseSection.focus=false;
                               moduleField.z=1
                               mouseSection.z=0
                           }
                        }
                    }
                }
            }
            Column {
                spacing: 5
                MyButton {
                    text: "Supprimer"
                    anchors.right: parent.right
                    onClicked: {
                        moduleModel.deleteSelected()
                    }
                }
            }
        }
    }
}
