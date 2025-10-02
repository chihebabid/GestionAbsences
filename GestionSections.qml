import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import "qrc:/qml/."

Rectangle {
    visible: true
    color: "transparent"
    MyGroupBox {
        width: parent.width*.8
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Gérer les sections")
        Column {
            spacing: 20
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.left: parent.left
            anchors.leftMargin: 20

            RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignVCenter
                MyTextField {
                    id: newSection
                    implicitWidth: 200
                    placeholderText: "Section"
                }

                MyButton {
                    text: "Ajouter une section"
                    onClicked: {
                        if (newSection.text !== "") {
                            sectionModel.insertSection(newSection.text)
                            newSection.text = ""
                        }
                    }
                }
            }
            Row {
                //width: root* .90
                spacing: 10
                ListView {
                    width: root.width * 0.6
                    height: 300
                    interactive: false
                    model: sectionModel
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }
                    delegate: Rectangle {
                        width: root.width * 0.6
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
                Column {
                    spacing: 5
                    MyButton {
                        text: "Supprimer"
                        anchors.right: parent.right
                        onClicked: {
                            sectionModel.deleteSelected()
                        }
                    }
                }
            }
        }
    }
}
