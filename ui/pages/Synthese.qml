import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/qml/."
import "../components"

Rectangle {
    id: syntheseInterface
    color: "transparent"

    MyText {
        text: "Année universitaire : " + databaseManager.educationYear[0]+" - "+databaseManager.educationYear[1]
        font.pixelSize: 18
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    // Sélection du semestre, de la section et du module
    MyGroupBox {
        id: groupSemestreSynthese
        width: root.width * .8
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        title: qsTr("Sélectionner le module")

        function updateListeModule() {
            if (semestreComboSynthese.currentIndex >= 0
                    && lSectionsSynthese.currentIndex >= 0) {
                let sectionIndex = lSectionsSynthese.currentIndex
                let sectionId = lSectionsSynthese.model.getSectionId(
                        sectionIndex)
                let semestre = semestreComboSynthese.model[semestreComboSynthese.currentIndex]
                lModulesSynthese.model.loadModulesForSection(sectionId,
                                                             semestre)
            }
        }

        function sendTypesCours(moduleId) {
            Qt.callLater(() => {
                             const selectedTypeIds = []
                             for (var i = 0; i < layoutCheckBoxes.children.length; ++i) {
                                 let cb = layoutCheckBoxes.children[i]
                                 if (cb.enabled && cb.checked) {
                                     selectedTypeIds.push(cb.typeId)
                                 }
                             }
                              summaryTableModel.loadAbsencesListForModule(
                                  moduleId, selectedTypeIds)
                         })
        }

        function updateListeSeance() {
            if (lModulesSynthese.currentIndex >= 0
                    && semestreComboSynthese.currentIndex >= 0
                    && lSectionsSynthese.currentIndex >= 0) {
                console.log("Semestre " + semestreComboSynthese.currentIndex
                            + " section: " + lSectionsSynthese.currentIndex
                            + " module: " + lModulesSynthese.currentIndex)
                let moduleId = lModulesSynthese.model.getId(
                        lModulesSynthese.currentIndex)
                courseTypeModel.loadCourseTypesForModule(moduleId)
                sendTypesCours(moduleId)
            }
        }

        Column {
            spacing: 10
            ModuleModel {
                id: moduleModelSynthese
            }

            Row {
                spacing: 10
                MyText {
                    text: "Semestre :"
                }

                MyComboBox {
                    id: semestreComboSynthese
                    width: groupSemestreSynthese.width * 0.18
                    model: [1, 2]
                    onCurrentIndexChanged: {
                        groupSemestreSynthese.updateListeModule()
                        groupSemestreSynthese.updateListeSeance()
                    }
                }

                Espacement {
                    width: groupSemestreSynthese.width * 0.15
                }

                MyText {
                    text: "Section :"
                }

                MyComboBox {
                    id: lSectionsSynthese
                    textRole: "name"
                    width: groupSemestreSynthese.width * 0.2
                    model: sectionModel
                    onCurrentIndexChanged: {
                        groupSemestreSynthese.updateListeModule()
                        Qt.callLater(() => {
                                         if (lModulesSynthese.count > 0) {
                                             lModulesSynthese.currentIndex = 0 // facultatif
                                             groupSemestreSynthese.updateListeSeance()
                                         }
                                     })
                    }
                }
            }

            Row {
                spacing: 10
                MyText {
                    text: "Module :"
                }

                MyComboBox {
                    id: lModulesSynthese
                    textRole: "name"
                    width: root.width * 0.4
                    model: moduleModelSynthese
                    onCurrentIndexChanged: {
                        groupSemestreSynthese.updateListeSeance()
                    }
                }
            }
        }
    }
    // Types de séances
    MyGroupBox {
        id: groupTypesSeances
        anchors.top: groupSemestreSynthese.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        title: qsTr("Types de séances")
        // Use C++ provided CourseTypeModel via context property 'courseTypeModel'
        Connections {
               target: databaseManager
               function onDatabaseReady() {
                    console.log("Load sessions from DB... console log ")
                    courseTypeModel.load()
               }
        }
        ScrollView {
            anchors.fill: parent
            contentWidth: parent.width
            clip: true

            Row {
                id: layoutCheckBoxes
                width: parent.width
                spacing: 8

                Repeater {
                    model: courseTypeModel
                    delegate: CheckBox {
                        text: model.name
                        property int typeId: model.id
                        enabled: model.hasSessions
                        checked: model.hasSessions
                        onCheckedChanged: {
                            console.log("Type selected:", text, "ID:", typeId, "State:", checked)
                            let moduleId = lModulesSynthese.model.getId(
                                    lModulesSynthese.currentIndex)
                            groupSemestreSynthese.sendTypesCours(moduleId)
                        }
                    }
                }
            }
        }
    }
    // Synthèse des absences
    MyGroupBox {
        id: groupSynthese
        anchors.top: groupTypesSeances.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        title: qsTr("Synthèse")
        //property var tableWidth: width * .85

        RowLayout {
            anchors.fill: parent
            implicitWidth: parent.width
            anchors.margins: 0
            spacing: 6
            ColumnLayout {
                id: myColumnSynthese
                Layout.fillWidth: true
                Layout.preferredWidth: parent.width * 0.85 // 85% pour la colonne gauche
                Layout.fillHeight: true // Remplit la hauteur
                anchors.margins: 0
                spacing: 1
                property var columnWidths: [myColumnSynthese.width * .15, myColumnSynthese.width
                    * .2, myColumnSynthese.width * .2, myColumnSynthese.width * .225, myColumnSynthese.width * .225-4]
                function columnWidth(col) {
                    return columnWidths[col]
                }
                Row {
                    spacing: 5
                    anchors.margins: 5
                    Text {
                        id: textNbSessions
                        font.pixelSize: 16
                        text: summaryTableModel ? qsTr("Number of sessions: ")
                                                   + summaryTableModel.nbSessions + " / "
                                                   + summaryTableModel.nbTotalSessions : ""
                    }
                }
                Item {
                    height: 6
                }

                Row {
                    id: headerSynthese
                    Layout.fillWidth: true
                    spacing: 1
                    height: 40
                    width: parent.width

                    Repeater {
                        model: ["N˚ inscription", "Nom", "Prénom", "Nombre d'absences", "Pourcentage"]
                        Rectangle {
                            width: myColumnSynthese.columnWidth(index)
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
                // Tableau
                TableView {
                    id: tableAbsencesSynthese
                    interactive: true
                    boundsBehavior: Flickable.StopAtBounds
                    flickableDirection: Flickable.AutoFlickIfNeeded
                    pressDelay: 999999
                    leftMargin: 0
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columnSpacing: 2
                    rowSpacing: 1
                    clip: true
                    model: summaryTableModel
                    height: 300
                    z: 2
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                    }

                    columnWidthProvider: function (col) {
                        return myColumnSynthese.columnWidth(col);
                    }

                    delegate: Rectangle {
                        implicitWidth: -1
                        implicitHeight: 30
                        border.color: "#d0d0d0"
                        border.width: 1
                        required property int row
                        property int presenceId: model.presence
                        color: {
                            let p = Number(model.percentage)
                            if (p <= 15)
                                return "#d0f5d0" // Vert clair
                            else if (p <= 30)
                                return "#ffe5b4" // Orange clair
                            else
                                return "#f8e0e0" // Rouge clair
                        }
                        Item {
                            anchors.fill: parent

                            Loader {
                                anchors.fill: parent
                                sourceComponent: textItem
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
                                            return model.lastName
                                        case 2:
                                            return model.firstName
                                        case 3:
                                            return model.presence
                                        case 4:
                                            return Number(
                                                        model.percentage).toFixed(
                                                        2) + " %"
                                        default:
                                            return ""
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // Colonne de droite pour le bouton "Imprimer"
            ColumnLayout {
                spacing: 10
                Item {
                    height: 60
                }

                MyButton {
                    id: btnImprimerSynthese
                    text: "Imprimer"
                    enabled: true
                    onClicked: {
                        printerManage.startPrinting("synthese")
                    }
                }
                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                }

            }
        }
    }
}

