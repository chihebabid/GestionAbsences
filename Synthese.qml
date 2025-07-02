import QtQuick
import QtQuick.Controls

Rectangle {
    id: welcomeInterface
    color: "transparent"

    Text {
        text: "Année universitaire : " + yearMonthString
        font.pixelSize: 18
        color: "black"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    // Section + Semestre
    GroupBox {
        id: groupSemestreSynthese
        width: root.width * .8
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Sélectionner le module")
        Column {
            spacing: 10

            Row {
                spacing: 10
                Text {
                    text: "Semestre :"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: semestreComboSynthese
                    width: groupSemestreSynthese.width * 0.18
                    model: [1, 2]
                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            let sectionIndex = lSectionsSynthese.currentIndex
                            let sectionId = lSectionsSynthese.model.getSectionId(
                                    sectionIndex)
                            let semestre = model[currentIndex]
                            lModulesSynthese.model.loadModulesForSection(sectionId,
                                                                 semestre)
                        }
                    }
                }

                Espacement {
                    width: groupSemestreSynthese.width * 0.15
                }

                Text {
                    text: "Section :"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: lSectionsSynthese
                    textRole: "name"
                    width: groupSemestreSynthese.width * 0.5
                    model: sectionModel
                    onCurrentIndexChanged: {

                        if (currentIndex >= 0) {
                            let sectionIndex = currentIndex
                            let sectionId = model.getSectionId(sectionIndex)
                            let semestre = semestreComboSynthese.model[semestreComboSynthese.currentIndex]
                            lModulesSynthese.model.loadModulesForSection(sectionId,
                                                                 semestre)
                        }
                    }
                }
            }
            Row {
                spacing: 10
                Text {
                    text: "Module :"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: lModulesSynthese
                    textRole: "name"
                    width: root.width * 0.4
                    model: moduleModel
                    onCurrentIndexChanged: {

                    }
                }
            }
        }
    }
}
