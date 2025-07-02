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

    // Section + Semestre
    GroupBox {
        id: groupSemestre
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
                id: wSemestreCombo
                width: groupSemestre.width * 0.18
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

            Espacement {
                width: groupSemestre.width * 0.15
            }

            Text {
                text: "Section :"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
            }



            ComboBox {
                id: wLSections
                textRole: "name"
                width: groupSemestre.width * 0.5
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


        }
    }
    GroupBox {
        id: groupSeance
        anchors.top: groupSemestre.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Planifier une séance")
        Column {
            spacing: 20

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
                    onCurrentIndexChanged: {
                        if (currentIndex !== -1)
                            btnPlanifier.enabled = true
                        else
                            btnPlanifier.enabled = false
                    }
                }
                Espacement {}
                Text {
                    text: "Type :"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
                    id: typeCoursCombo
                    textRole: "nom"
                    width: root.width * 0.1
                    model: typeCoursModel
                    currentIndex: 0 // Valeur par défaut

                    onCurrentIndexChanged: {
                        console.log("Type de cours sélectionné :",
                                    typeCoursCombo.currentText)
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
                    from: 8 * 60 // 08:00 en minutes
                    to: 18 * 60 // 18:00 en minutes
                    stepSize: 15 // Pas de 15 minutes
                    value: from // Initialiser à 08:00
                    // Affichage formaté
                    textFromValue: function (value, locale) {
                        let h = Math.floor(value / 60)
                        let m = value % 60
                        return Qt.formatTime(new Date(0, 0, 0, h, m), "hh:mm")
                    }

                    valueFromText: function (text, locale) {
                        let parts = text.split(":")
                        return parseInt(parts[0]) * 60 + parseInt(parts[1])
                    }

                    onValueChanged: {
                        console.log("Heure sélectionnée :",
                                    textFromValue(value))
                    }
                    Component.onCompleted: {
                        // Heure courante
                        let now = new Date()
                        let currentMinutes = now.getHours(
                                ) * 60 + now.getMinutes()

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
                    from: 0 // 08:00 en minutes
                    to: 3 * 60 // 18:00 en minutes
                    stepSize: 15 // Pas de 15 minutes
                    value: from // Initialiser à 08:00
                    // Affichage formaté
                    textFromValue: function (value, locale) {
                        let h = Math.floor(value / 60)
                        let m = value % 60
                        return Qt.formatTime(new Date(0, 0, 0, h, m), "hh:mm")
                    }

                    valueFromText: function (text, locale) {
                        let parts = text.split(":")
                        return parseInt(parts[0]) * 60 + parseInt(parts[1])
                    }

                    onValueChanged: {
                        console.log("Heure sélectionnée :",
                                    textFromValue(value))
                    }
                    Component.onCompleted: {
                        value = 90
                    }
                }
            }
            Item {
                width: parent.width
                height: 40

                Button {
                    id: btnPlanifier
                    text: "Planifier"
                    width: root.width * 0.1
                    anchors.right: parent.right
                    enabled: false
                    onClicked: {
                        if (wModules.currentIndex !== -1) {
                            let idModule = wModules.model.getId(
                                    wModules.currentIndex)
                            let idTypeCours = typeCoursCombo.model.getId(
                                    typeCoursCombo.currentIndex)

                            let date = dateField.text // format: "dd/MM/yyyy"

                            let h = Math.floor(timeSpin.value / 60)
                            let m = timeSpin.value % 60
                            let heureDebut = Qt.formatTime(new Date(0, 0, 0, h,
                                                                    m), "hh:mm")

                            let duree = dureeSpin.value
                            planifierSeance.ajouterSeance(idModule,
                                                          idTypeCours, date,
                                                          heureDebut, duree)
                            listeSeances.model.loadSeances()
                            console.log("Planification réussie...")
                        }
                    }
                }
            }
        }
    }
    GroupBox {
        id: groupListe
        property var tableWidth: width * .8
        property var columnWidths: [tableWidth * .2, tableWidth * .3, tableWidth
            * .3, tableWidth * .2]

        anchors.top: groupSeance.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: groupSeance.width
        title: qsTr("Marquer les absences")
        function columnWidth(col) {
            return columnWidths[col]
        }
        RowLayout {
            anchors.fill: parent
            //anchors.margins: 10
            spacing: 0
            ColumnLayout {
                //spacing: 10
                //anchors.fill: parent
                //anchors.margins: 10
                spacing: 0
                RowLayout {
                    id: selectionSeance
                    spacing: 10
                    Text {
                        text: "Séance :"
                        font.pixelSize: 16
                        verticalAlignment: Text.AlignVCenter
                    }

                    ComboBox {
                        id: listeSeances
                        //Layout.fillWidth: true
                        Layout.preferredWidth: root.width * .4
                        model: seanceModel

                        onCurrentIndexChanged: {
                            btnSupprimer.enabled = currentIndex === -1 ? false : true
                            btnImprimer.enabled = btnSupprimer.enabled
                            absenceModel ? absenceModel.loadEtudiantsForSeance(
                                               seanceModel.getId(
                                                   currentIndex)) : -1
                        }
                    }
                    Button {
                        id: btnSupprimer
                        text: "Supprimer"
                        enabled: false
                        //anchors.margins: 20
                        Layout.alignment: Qt.AlignRight
                        //Layout.margins: 20
                        onClicked: {

                        }
                    }
                    Rectangle {
                        width: 19
                        height: 60
                        color: "red"
                    }
                }
                RowLayout {
                    id: header
                    spacing: 1
                    Rectangle {
                        width: 19
                        height: 30
                        color: "red"
                    }
                    Repeater {
                        model: ["N˚ inscription", "Nom", "Prénom", "Présence"]
                        Rectangle {
                            Layout.preferredWidth: groupListe.columnWidth(index)
                            height: 30
                            color: "#f0f0f0"
                            border.color: "#ccc"
                            border.width: 1

                            Text {
                                anchors.fill: parent
                                anchors.centerIn: parent
                                horizontalAlignment: Text.AlignHCenter
                                text: modelData
                                font.bold: true
                                font.pixelSize: 14
                            }
                        }
                    }
                }

                TableView {
                    id: tableAbsences
                    interactive: false
                    leftMargin: 20
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    columnSpacing: 1
                    rowSpacing: 1
                    //clip: true
                    model: absenceModel
                    height: 200
                    z: 2
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }
                    columnWidthProvider: function (col) {
                        return groupListe.columnWidths[col]
                    }

                    delegate: Rectangle {
                        implicitWidth: 150
                        implicitHeight: 30
                        border.color: "#ccc"
                        border.width: 1
                        required property int row
                        property int presenceId: model.presence
                        Item {
                            anchors.fill: parent
                            // Colonne 0 à 2 → simple texte
                            // Colonne 3 → ComboBox
                            Loader {
                                anchors.fill: parent
                                sourceComponent: column === 3 ? componentPresence : textItem
                            }

                            Component {
                                id: textItem
                                Text {
                                    anchors.centerIn: parent
                                    font.pixelSize: 14
                                    text: {
                                        switch (column) {
                                        case 0:
                                            return model.inscri
                                        case 1:
                                            return model.nom
                                        case 2:
                                            return model.prenom
                                        default:
                                            return ""
                                        }
                                    }
                                }
                            }

                            Component {
                                id: componentPresence
                                ComboBox {
                                    id: comboPresence
                                    anchors.fill: parent
                                    model: presenceModel
                                    textRole: "label"

                                    // index de l'élément actuel dans presenceModel (à partir de model.presenceId)
                                    currentIndex: presenceModel ? presenceModel.getIndexById(
                                                                      presenceId) : -1
                                    onCurrentIndexChanged: {
                                        if (presenceModel) {
                                            const newId = presenceModel.getId(
                                                            currentIndex)
                                            absenceModel.setPresence(row, newId)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            ColumnLayout {
                spacing: 15
                Button {
                    id: btnImprimer
                    text: "Imprimer"                    
                    Layout.alignment: Qt.AlignRight
                    enabled: false
                    onClicked: {
                        printerManage.startPrinting("absence")
                    }
                }
            }
        }
    }
}
