import QtQuick
import QtQuick.Controls

Rectangle {
    id: welcomeInterface
   // Layout.fillWidth: true
   // Layout.fillHeight: true

    Text {
        text: "Année universitaire : " + yearMonthString
        font.pixelSize: 18
        color: "black"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    // Section + ComboBox, centrés sous le texte
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
                id: wLSections
                textRole: "name"
                width: root.width * 0.3
                model: sectionModel
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
                id: wSemestreCombo
                model: [1, 2]
                onCurrentIndexChanged: {
                    if (currentIndex >= 0) {
                        let sectionIndex = wLSections.currentIndex
                        let sectionId = sectionModel.getSectionId(sectionIndex)
                        let semestre = wSemestreCombo.model[wSemestreCombo.currentIndex]
                        moduleModel.loadModulesForSection(sectionId, semestre)
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
                id: lModules
                textRole: "name"
                width: root.width * 0.4
                model: moduleModel
            }
        }
    }
}
