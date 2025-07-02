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
        width: groupSeance.width
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Sélectionner la section")
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
                        let sectionIndex = wLSections.currentIndex
                        let sectionId = wLSections.model.getSectionId(
                                sectionIndex)
                        let semestre = model[currentIndex]
                        wModules.model.loadModulesForSection(sectionId,
                                                             semestre)
                    }
                }
            }

            Espacement {
                width: groupSemestre.width * 0.15
            }

            Text {
                text: "Section :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }



            ComboBox {
                id: lSectionsSynthese
                textRole: "name"
                width: groupSemestre.width * 0.5
                model: sectionModel
                onCurrentIndexChanged: {

                    if (currentIndex >= 0) {
                        let sectionIndex = currentIndex
                        let sectionId = model.getSectionId(
                                sectionIndex)
                        let semestre = wSemestreCombo.model[semestreComboSynthese.currentIndex]
                        wModules.model.loadModulesForSection(sectionId,
                                                             semestre)
                    }
                }
            }


        }
    }
}
