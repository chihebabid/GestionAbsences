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
                        if (currentIndex >= 0) {
                            let sectionIndex = lSectionsSynthese.currentIndex
                            let sectionId = lSectionsSynthese.model.getSectionId(
                                    sectionIndex)
                            let semestre = model[currentIndex]
                            lModulesSynthese.model.loadModulesForSection(
                                        sectionId, semestre)
                        }
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

                        if (currentIndex >= 0) {
                            let sectionIndex = currentIndex
                            let sectionId = model.getSectionId(sectionIndex)
                            let semestre = semestreComboSynthese.model[semestreComboSynthese.currentIndex]
                            lModulesSynthese.model.loadModulesForSection(
                                        sectionId, semestre)
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
                    id: lModulesSynthese
                    textRole: "name"
                    width: root.width * 0.4
                    model: moduleModelSynthese
                    onCurrentIndexChanged: {
                        if (currentIndex !== -1) {
                            let moduleId = model.getId(currentIndex)
                            typeCoursModel.loadTypesCoursForModule(moduleId)
                        }
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
                        onCheckedChanged: {
                            console.log("Type sélectionné:", text, "ID:",
                                        typeId, "État:", checked)
                            // Vous pouvez appeler une méthode C++ ici si nécessaire
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
        property var tableWidth: width * .9
        property var columnWidths: [tableWidth * .15, tableWidth
            * .2, tableWidth * .2, tableWidth * .225, tableWidth * .225]
        function columnWidth(col) {
            return columnWidths[col]
        }
        ColumnLayout {
            RowLayout {
                id: headerSynthese
                spacing: 1
                Rectangle {
                    width: 19
                    height: 30
                    color: "red"
                }

                Repeater {
                    model: ["N˚ inscription", "Nom", "Prénom", "Nombre de séances", "Nombre d'absences"]
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
    }
}
