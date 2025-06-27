import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {

    width: 320
    height: 360
    modal: true
    focus: true
    anchors.centerIn: Overlay.overlay

    property date selectedDate: new Date()
    property int currentMonth: selectedDate.getMonth()
    property int currentYear: selectedDate.getFullYear()

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        // Barre de navigation avec mois et année
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 10

            Button {
                text: "<"
                onClicked: {
                    if (calendarPopup.currentMonth === 0) {
                        calendarPopup.currentMonth = 11
                        calendarPopup.currentYear--
                    } else {
                        calendarPopup.currentMonth--
                    }
                }
            }

            Label {
                text: {
                    // Créez une date JS
                    var date = new Date(calendarPopup.currentYear, calendarPopup.currentMonth, 1)

                    // Utilisez Qt.locale() pour obtenir les paramètres régionaux de l'utilisateur
                    var locale = Qt.locale()

                    // Formatage selon la locale
                    return date.toLocaleDateString(locale, "MMMM yyyy")
                }
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }
            Button {
                text: ">"
                onClicked: {
                    if (calendarPopup.currentMonth === 11) {
                        calendarPopup.currentMonth = 0
                        calendarPopup.currentYear++
                    } else {
                        calendarPopup.currentMonth++
                    }
                }
            }
        }

        // Affichage des jours de la semaine
        DayOfWeekRow {
            Layout.fillWidth: true
            locale: Qt.locale()
        }

        // Grille des jours
        MonthGrid {
            id: grid
            Layout.fillWidth: true
            Layout.fillHeight: true

            month: calendarPopup.currentMonth
            year: calendarPopup.currentYear
            locale: Qt.locale()

            onClicked: (date) => {
                calendarPopup.selectedDate = date
                calendarPopup.close()
            }

            delegate: Rectangle {
                width: 40
                height: 40
                color: model.date.getDate() === calendarPopup.selectedDate.getDate()
                       && model.date.getMonth() === calendarPopup.selectedDate.getMonth()
                       && model.date.getFullYear() === calendarPopup.selectedDate.getFullYear()
                       ? "#87ceeb" : "transparent"
                border.color: "#ccc"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: model.day
                    font.bold: model.date.getDay() === 0  // Mettre en gras le dimanche
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: grid.clicked(model.date)
                }
            }
        }
    }
}
