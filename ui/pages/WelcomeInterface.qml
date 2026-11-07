import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.enhancetech.absences
import "qrc:/qml/."
import "../components"
import "../dialogs"
Rectangle {
    id: welcomeInterface
    width: parent.width
    height: parent.height
    color: "transparent"

    // Modèles et services non visuels
    ModuleModel {
        id: moduleModel
    }

    SessionPlanner {
        id: sessionPlanner
    }

    PopupDate {
        id: calendarPopup
    }

    // Recharger les modules selon le semestre et la section sélectionnés
    function updateListeModules() {
        if (wSemestreCombo.currentIndex >= 0 && wLSections.currentIndex >= 0) {
            let sectionId = wLSections.model.getSectionId(
                    wLSections.currentIndex)
            let semestre = wSemestreCombo.model[wSemestreCombo.currentIndex]
            wModules.model.loadModulesForSection(sectionId, semestre)
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
            id: groupSemestre
            Layout.fillWidth: true
            title: qsTr("Sélectionner la section")

            RowLayout {
                width: parent.width
                spacing: 8

                MyText {
                    text: "Semestre :"
                }

                MyComboBox {
                    id: wSemestreCombo
                    Layout.preferredWidth: 100
                    model: [1, 2]
                    onCurrentIndexChanged: welcomeInterface.updateListeModules()
                }

                Espacement {}

                MyText {
                    text: "Section :"
                }

                MyComboBox {
                    id: wLSections
                    Layout.fillWidth: true
                    textRole: "name"
                    model: sectionModel
                    onCurrentIndexChanged: welcomeInterface.updateListeModules()
                }
            }
        }

        // Planification d'une séance
        MyGroupBox {
            id: groupSeance
            Layout.fillWidth: true
            title: qsTr("Planifier une séance")

            ColumnLayout {
                width: parent.width
                spacing: 8

                // Module et type de cours
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    MyText {
                        text: "Module :"
                    }

                    MyComboBox {
                        id: wModules
                        Layout.fillWidth: true
                        textRole: "name"
                        model: moduleModel
                        onCurrentIndexChanged: {
                            btnPlanifier.enabled = currentIndex !== -1
                        }
                    }

                    MyText {
                        text: "Type :"
                    }

                    MyComboBox {
                        id: courseTypeCombo
                        Layout.preferredWidth: 120
                        textRole: "name"
                        model: courseTypeModel
                        currentIndex: 0
                        onCurrentIndexChanged: {
                            console.log("Type de cours sélectionné :",
                                        courseTypeCombo.currentText)
                        }
                    }
                }

                // Date, horaire et validation
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    MyText {
                        text: "Date :"
                    }

                    MyTextField {
                        id: dateField
                        Layout.preferredWidth: 120
                        text: Qt.formatDate(new Date(), "dd/MM/yyyy")
                        MouseArea {
                            anchors.fill: dateField
                            //cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                calendarPopup.open()
                            }
                        }
                    }

                    MyText {
                        text: "Heure début :"
                    }

                    MySpinBox {
                        id: timeSpin
                        Layout.preferredWidth: 120
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

                    MyText {
                        text: "Durée :"
                    }

                    MySpinBox {
                        id: dureeSpin
                        Layout.preferredWidth: 120
                        from: 0 // 0 minute
                        to: 3 * 60 // 3 heures en minutes
                        stepSize: 15 // Pas de 15 minutes
                        value: from
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
                            console.log("Durée sélectionnée :",
                                        textFromValue(value))
                        }
                        Component.onCompleted: {
                            value = 90
                        }
                    }

                    // Pousse le bouton vers la droite
                    Item {
                        Layout.fillWidth: true
                    }

                    MyButton {
                        id: btnPlanifier
                        text: "Planifier"
                        enabled: false
                        onClicked: {
                            if (wModules.currentIndex !== -1) {
                                let idModule = wModules.model.getId(
                                        wModules.currentIndex)
                                let idCourseType = courseTypeCombo.model.getId(
                                        courseTypeCombo.currentIndex)

                                let date = dateField.text // format: "dd/MM/yyyy"

                                let h = Math.floor(timeSpin.value / 60)
                                let m = timeSpin.value % 60
                                let heureDebut = Qt.formatTime(new Date(0, 0, 0, h,
                                                                        m), "hh:mm")

                                let duree = dureeSpin.value
                                sessionPlanner.addSession(idModule,
                                                          idCourseType, date,
                                                          heureDebut, duree)
                                listeSeances.model.loadSessions()
                                console.log("Planification réussie...")
                            }
                        }
                    }
                }
            }
        }

        // Liste des séances et marquage des absences
        MyGroupBox {
            id: lSessions
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 220
            title: qsTr("Marquer les absences")

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                // Sélection de la séance et actions
                RowLayout {
                    id: selectionSeance
                    Layout.fillWidth: true
                    spacing: 8

                    MyText {
                        text: "Séance :"
                    }

                    MyComboBox {
                        id: listeSeances
                        Layout.fillWidth: true
                        model: sessionModel

                        onCurrentIndexChanged: {
                            btnSupprimer.enabled = currentIndex !== -1
                            btnImprimer.enabled = btnSupprimer.enabled
                            if (model) {
                                model.setSelectedIndex(currentIndex)
                            }
                            // Aucune séance sélectionnée (ex. après rechargement du modèle) : on vide la liste
                            if (absenceModel) {
                                absenceModel.loadEtudiantsForSeance(
                                            currentIndex !== -1 ? sessionModel.getId(
                                                                      currentIndex) : -1)
                            }
                        }
                    }

                    MyButton {
                        id: btnSupprimer
                        text: "Supprimer"
                        enabled: false
                        Layout.preferredHeight: listeSeances.height
                        topInset: 0
                        bottomInset: 0
                        onClicked: {

                        }
                    }

                    MyButton {
                        id: btnImprimer
                        text: "Imprimer"
                        enabled: false
                        Layout.preferredHeight: listeSeances.height
                        topInset: 0
                        bottomInset: 0
                        onClicked: {
                            printerManage.startPrinting("absence")
                        }
                    }
                }

                // Tableau des absences (en-tête + lignes)
                ColumnLayout {
                    id: myColumnAbsences
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 1
                    // Largeurs des colonnes, espacement entre colonnes déduit
                    readonly property int columnSpacing: 2
                    property var columnWidths: {
                        let w = myColumnAbsences.width - 3 * columnSpacing
                        return [w * .2, w * .3, w * .3, w * .2]
                    }

                    Row {
                        id: header
                        Layout.fillWidth: true
                        height: 36
                        spacing: myColumnAbsences.columnSpacing
                        Repeater {
                            model: ["N˚ inscription", "Nom", "Prénom", "Présence"]
                            Rectangle {
                                width: myColumnAbsences.columnWidths[index]
                                height: parent.height
                                color: "#0078d4"
                                border.color: "#d0d0d0"
                                border.width: 1
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData
                                    font.pixelSize: 14
                                    font.bold: true
                                    color: "#ffffff"
                                }
                            }
                        }
                    }

                    MyTableView {
                        id: tableAbsences
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        leftMargin: 0
                        rightMargin: 0
                        columnSpacing: myColumnAbsences.columnSpacing
                        rowSpacing: 1
                        model: absenceModel

                        delegate: Rectangle {
                            implicitHeight: 30
                            implicitWidth: -1
                            border.color: "#d0d0d0"
                            border.width: 1
                            required property int row
                            property int presenceId: model.presence
                            Item {
                                anchors.fill: parent
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
                                    MyComboBox {
                                        id: presenceCombo
                                        anchors.fill: parent
                                        textRole: "label"
                                        model: presenceModel

                                        // index de l'élément actuel dans presenceModel (à partir de model.presenceId)
                                        currentIndex: {
                                            return presenceModel ? presenceModel.getIndexById(presenceId) : -1;
                                        }
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
            }
        }
    }
}
