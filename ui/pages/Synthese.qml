import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/qml/."
import "../components"

Rectangle {
    id: syntheseInterface
    color: "transparent"

    // Modèle non visuel des modules
    ModuleModel {
        id: moduleModelSynthese
    }

    // Use C++ provided CourseTypeModel via context property 'courseTypeModel'
    Connections {
        target: databaseManager
        function onDatabaseReady() {
            console.log("Load sessions from DB... console log ")
            courseTypeModel.load()
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
            text: "Année universitaire : " + databaseManager.educationYear[0]+" - "+databaseManager.educationYear[1]
            font.pixelSize: 18
        }

        // Sélection du semestre, de la section, du module et des types de séances
        MyGroupBox {
            id: groupSemestreSynthese
            Layout.fillWidth: true
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

            ColumnLayout {
                width: parent.width
                spacing: 8

                // Semestre, section et module
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    MyText {
                        text: "Semestre :"
                    }

                    MyComboBox {
                        id: semestreComboSynthese
                        Layout.preferredWidth: 100
                        model: [1, 2]
                        onCurrentIndexChanged: {
                            groupSemestreSynthese.updateListeModule()
                            groupSemestreSynthese.updateListeSeance()
                        }
                    }

                    MyText {
                        text: "Section :"
                    }

                    MyComboBox {
                        id: lSectionsSynthese
                        Layout.fillWidth: true
                        Layout.horizontalStretchFactor: 1
                        textRole: "name"
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

                    MyText {
                        text: "Module :"
                    }

                    MyComboBox {
                        id: lModulesSynthese
                        Layout.fillWidth: true
                        Layout.horizontalStretchFactor: 2
                        textRole: "name"
                        model: moduleModelSynthese
                        onCurrentIndexChanged: {
                            groupSemestreSynthese.updateListeSeance()
                        }
                    }
                }

                // Types de séances pris en compte dans la synthèse
                RowLayout {
                    id: groupTypesSeances
                    Layout.fillWidth: true
                    spacing: 8

                    MyText {
                        text: "Types de séances :"
                    }

                    Flow {
                        id: layoutCheckBoxes
                        Layout.fillWidth: true
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
        }

        // Synthèse des absences
        MyGroupBox {
            id: groupSynthese
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 220
            title: qsTr("Synthèse")

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                // Nombre de séances, légende des couleurs et impression
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    MyText {
                        id: textNbSessions
                        leftPadding: 0
                        font.pixelSize: 16
                        font.bold: true
                        text: summaryTableModel ? qsTr("Nombre de séances : ")
                                                   + summaryTableModel.nbSessions + " / "
                                                   + summaryTableModel.nbTotalSessions : ""
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    // Légende : mêmes seuils que les couleurs des lignes du tableau
                    Repeater {
                        model: [{ couleur: "#d0f5d0", libelle: "≤ 15 %" },
                                { couleur: "#ffe5b4", libelle: "≤ 30 %" },
                                { couleur: "#f8e0e0", libelle: "> 30 %" }]
                        Row {
                            spacing: 4
                            Rectangle {
                                width: 14
                                height: 14
                                anchors.verticalCenter: parent.verticalCenter
                                color: modelData.couleur
                                border.color: "#d0d0d0"
                                border.width: 1
                            }
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: modelData.libelle
                                font.pixelSize: 12
                                color: "#555555"
                            }
                        }
                    }

                    MyButton {
                        id: btnImprimerSynthese
                        text: "Imprimer"
                        enabled: true
                        onClicked: {
                            printerManage.startPrinting("synthese")
                        }
                    }
                }

                // Tableau de synthèse (en-tête + lignes)
                ColumnLayout {
                    id: myColumnSynthese
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 1
                    // Largeurs des colonnes, espacement entre colonnes déduit
                    readonly property int columnSpacing: 2
                    property var columnWidths: {
                        let w = myColumnSynthese.width - 4 * columnSpacing
                        return [w * .15, w * .2, w * .2, w * .225, w * .225]
                    }
                    function columnWidth(col) {
                        return columnWidths[col]
                    }

                    Row {
                        id: headerSynthese
                        Layout.fillWidth: true
                        spacing: myColumnSynthese.columnSpacing
                        height: 36

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
                        columnSpacing: myColumnSynthese.columnSpacing
                        rowSpacing: 1
                        clip: true
                        model: summaryTableModel
                        z: 2
                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }

                        columnWidthProvider: function (col) {
                            return myColumnSynthese.columnWidth(col);
                        }

                        delegate: Rectangle {
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
            }
        }
    }
}
