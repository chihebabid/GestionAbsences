import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: welcomeInterface
    color: "transparent"

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
            Popup {
                id: calendarPopup
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


                    // Barre de navigation
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
                            text: Qt.formatDate(new Date(currentYear, currentMonth, 1), "MMMM yyyy")
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

                    // En-tête jours + grille
                    GridLayout {
                        columns: 2
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        DayOfWeekRow {
                            Layout.column: 1
                            Layout.fillWidth: true
                            locale: Qt.locale()
                        }

                        WeekNumberColumn {
                            Layout.fillHeight: true
                            month: calendarPopup.currentMonth
                            year: calendarPopup.currentYear
                            locale: Qt.locale()
                        }

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
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: grid.clicked(model.date)
                                }
                            }
                        }
                    }
                }
            }

            Text {
                text: "Heure début :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                text: "Durée :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
