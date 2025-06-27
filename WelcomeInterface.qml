import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
                onCurrentIndexChanged: {

                    if (wLSections.currentIndex >= 0) {
                        let sectionIndex = wLSections.currentIndex
                        let sectionId = wLSections.model.getSectionId(
                                sectionIndex)
                        let semestre = wSemestreCombo.model[wSemestreCombo.currentIndex]
                        wModules.model.loadModulesForSection(sectionId,
                                                             semestre)
                    }
                }
            }
            Espacement {

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
                    if (wSemestreCombo.currentIndex >= 0) {
                        let sectionIndex = wLSections.currentIndex
                        let sectionId = wLSections.model.getSectionId(
                                sectionIndex)
                        let semestre = wSemestreCombo.model[wSemestreCombo.currentIndex]
                        wModules.model.loadModulesForSection(sectionId,
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
                id: wModules
                textRole: "name"
                width: root.width * 0.4
                model: moduleModel
            }
            Espacement {}
            Text {
                text: "Type :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            ComboBox {
                id: typeCoursCombo
                width: root.width * 0.1
                model: ["Cours", "CI", "TD", "TP"]
                currentIndex: 0  // Valeur par défaut

                onCurrentIndexChanged: {
                    console.log("Type de cours sélectionné :", typeCoursCombo.currentText)
                }
            }
        }
        Row {
            property date selectedDate: new Date()
            spacing: 10
            Text {
                text: "Date :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            TextField {
                id: dateField
                text: Qt.formatDate(parent.selectedDate, "dd/MM/yyyy")

                MouseArea {
                    anchors.fill: dateField
                    //cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        calendarPopup.open()
                    }
                }
            }
            PopupDate {
                id: calendarPopup
            }
            Espacement {}
            Text {
                text: "Heure début :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
                id: timeSpin
                from: 8 * 60         // 08:00 en minutes
                to: 18 * 60          // 18:00 en minutes
                stepSize: 15         // Pas de 15 minutes
                value: from          // Initialiser à 08:00
                // Affichage formaté
                textFromValue: function(value, locale) {
                    let h = Math.floor(value / 60)
                    let m = value % 60
                    return Qt.formatTime(new Date(0, 0, 0, h, m), "hh:mm")
                }

                valueFromText: function(text, locale) {
                    let parts = text.split(":")
                    return parseInt(parts[0]) * 60 + parseInt(parts[1])
                }

                onValueChanged: {
                    console.log("Heure sélectionnée :", textFromValue(value))
                }
                Component.onCompleted: {
                      // Heure courante
                      let now = new Date()
                      let currentMinutes = now.getHours() * 60 + now.getMinutes()

                      // Arrondi au multiple de 15 le plus proche
                      let rounded = Math.round(currentMinutes / 15) * 15

                      // Limiter dans l’intervalle autorisé
                      if (rounded < from)
                          value = from
                      else if (rounded > to)
                          value = to
                      else
                          value = rounded
                  }
            }
            Text {
                text: "Durée :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            SpinBox {
                id: dureeSpin
                from: 0         // 08:00 en minutes
                to: 3 * 60          // 18:00 en minutes
                stepSize: 15         // Pas de 15 minutes
                value: from          // Initialiser à 08:00
                // Affichage formaté
                textFromValue: function(value, locale) {
                    let h = Math.floor(value / 60)
                    let m = value % 60
                    return Qt.formatTime(new Date(0, 0, 0, h, m), "hh:mm")
                }

                valueFromText: function(text, locale) {
                    let parts = text.split(":")
                    return parseInt(parts[0]) * 60 + parseInt(parts[1])
                }

                onValueChanged: {
                    console.log("Heure sélectionnée :", textFromValue(value))
                }
                Component.onCompleted: {
                    value = 90
                  }
            }
        }
    }
}
