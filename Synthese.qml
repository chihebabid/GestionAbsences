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
        id: groupSemestreSynthese
        width: root.width * .8
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
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
                             syntheseTableModel.loadAbsencesListForModule(
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
                typeCoursModel.loadTypesCoursForModule(moduleId)
                sendTypesCours(moduleId)
            }
        }

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
                        groupSemestreSynthese.updateListeModule()
                        groupSemestreSynthese.updateListeSeance()
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
                Text {
                    text: "Module :"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }

                ComboBox {
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

    GroupBox {
        id: groupTypesSeances
        width: root.width * .8
        anchors.top: groupSemestreSynthese.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Types de séances")
        ScrollView {
            anchors.fill: parent
            contentWidth: parent.width
            clip: true

            Row {
                id: layoutCheckBoxes
                width: parent.width
                spacing: 8

                Repeater {
                    model: typeCoursModel
                    delegate: CheckBox {
                        text: model.nom
                        property int typeId: model.id
                        enabled: model.hasSeances
                        checked: model.hasSeances
                        onCheckedChanged: {
                            console.log("Type sélectionné:", text, "ID:",
                                        typeId, "État:", checked)
                            let moduleId = lModulesSynthese.model.getId(
                                    lModulesSynthese.currentIndex)
                            groupSemestreSynthese.sendTypesCours(moduleId)
                        }
                    }
                }
            }
        }
    }

    GroupBox {
        id: groupSynthese
        width: root.width * .8
        anchors.top: groupTypesSeances.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        title: qsTr("Synthèse")
        property var tableWidth: width * .85
        property var columnWidths: [tableWidth * .15, tableWidth
            * .2, tableWidth * .2, tableWidth * .225, tableWidth * .225]
        function columnWidth(col) {
            return columnWidths[col]
        }
        ColumnLayout {
            RowLayout {
                spacing: 10
                Text {
                    id: textNbSeances
                    font.pixelSize: 16
                    text: syntheseTableModel ? qsTr("Nombre de séances : ")
                                               + syntheseTableModel.nbSeances + " / "
                                               + syntheseTableModel.nbTotalSeances : ""
                }
            }
            Rectangle {
                width: 19
                height: 30
                color: "transparent"
            }
            RowLayout {
                id: headerSynthese
                spacing: 1

                Rectangle {
                    width: 19
                    height: 30
                    color: "transparent"
                }

                Repeater {
                    model: ["N˚ inscription", "Nom", "Prénom", "Nombre d'absences", "Pourcentage"]
                    Rectangle {
                        Layout.preferredWidth: groupSynthese.columnWidth(index)
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
            // Tableau
            TableView {
                id: tableAbsencesSynthese
                interactive: false
                leftMargin: 20
                Layout.fillWidth: true
                Layout.fillHeight: true

                columnSpacing: 2
                rowSpacing: 1
                //clip: true
                model: syntheseTableModel
                height: 300
                z: 2
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
                columnWidthProvider: function (col) {
                    return groupSynthese.columnWidths[col]
                }

                delegate: Rectangle {
                    implicitWidth: 150
                    implicitHeight: 30
                    border.color: "#ccc"
                    border.width: 1
                    required property int row
                    property int presenceId: model.presence
                    color: {
                            let p = Number(model.pourcentage)
                            if (p <= 15) return "#d0f5d0"       // Vert clair
                            else if (p <= 30) return "#ffe5b4"   // Orange clair
                            else return "#f8e0e0"               // Rouge clair
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
                                        return model.nom
                                    case 2:
                                        return model.prenom
                                    case 3:
                                        return model.presence
                                    case 4:
                                        return Number(model.pourcentage).toFixed(2) + " %"
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
