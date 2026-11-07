import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import com.enhancetech.absences
import "qrc:/qml/."
import "../components"


Rectangle {
    id: gestionEtudiants
    color: "transparent"

    FileDialog {
        id: fileDialog
        title: "Choisir un fichier"
        nameFilters: ["Fichiers csv (*.csv)", "Tous les fichiers (*)"]
        onAccepted: {
            console.log("Fichier sélectionné :", selectedFile);
            let sectionIndex = sectionCombo.currentIndex;
            let sectionId = sectionModel.getSectionId(sectionIndex);
            studentManager.importCSV(selectedFile, sectionId);
        }
        onRejected: {
            console.log("Sélection annulée");
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
            text: "Année universitaire : " + (databaseManager.educationYear)[0] + " - " + (databaseManager.educationYear)[1]
            font.pixelSize: 18
        }

        MyGroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("Gérer les étudiants")

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                // Sélection de la section et actions
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    MyText {
                        text: "Section :"
                    }

                    MyComboBox {
                        id: sectionCombo
                        Layout.preferredWidth: 250
                        textRole: "name"
                        model: sectionModel
                        onActivated: {
                            if (sectionCombo.currentIndex >= 0) {
                                bImporter.enabled = true;
                                let sectionIndex = sectionCombo.currentIndex;
                                console.log("Section id: " + sectionIndex);
                                let sectionId = sectionModel.getSectionId(sectionIndex);
                                studentManager.fetchForSection(sectionId);
                            }
                        }
                    }

                    // Pousse les boutons vers la droite
                    Item {
                        Layout.fillWidth: true
                    }

                    MyButton {
                        id: bImporter
                        text: "Importer"
                        enabled: false
                        onClicked: {
                            fileDialog.open();
                        }
                    }

                    MyButton {
                        id: bSupprimer
                        text: "Supprimer"
                        onClicked: {

                            // moduleModel.deleteSelected()
                        }
                    }
                }

                // Aide à l'utilisation du tableau
                Text {
                    text: "Clic : modifier une cellule · Ctrl + clic : sélectionner l'étudiant"
                    font.pixelSize: 12
                    color: "#555555"
                }

                // Tableau des étudiants (en-tête + lignes)
                ColumnLayout {
                    id: myColumnEtudiants
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 1
                    // Largeurs des colonnes, espacement entre colonnes déduit
                    readonly property int columnSpacing: 1
                    property var columnWidths: {
                        let w = myColumnEtudiants.width - 3 * columnSpacing
                        return [w * .2, w * .3, w * .3, w * .2]
                    }

                    Row {
                        id: header
                        Layout.fillWidth: true
                        height: 36
                        spacing: myColumnEtudiants.columnSpacing

                        Repeater {
                            model: ["N˚ inscription", "Nom", "Prénom", "E-mail"]
                            Rectangle {
                                width: myColumnEtudiants.columnWidths[index]
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
                        id: tableEtudiants
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        property bool toRevert: false
                        model: studentManager.mModel
                        leftMargin: 0
                        rightMargin: 0
                        columnSpacing: myColumnEtudiants.columnSpacing
                        interactive: false
                        delegate: Rectangle {
                            implicitHeight: 40
                            border.color: "#d0d0d0"
                            border.width: 1
                            color: model.selected ? "#87CEFA" : Qt.color("white")
                            required property int row
                            TextField {
                                id: sectionField
                                background: null
                                anchors.fill: parent
                                anchors.margins: 0
                                color: "black"
                                z: mouseSection.enabled ? 0 : 1  // Show when editing
                                text: {
                                    switch (column) {
                                        case 0:
                                            return model.inscri;
                                        case 1:
                                            return model.nom;
                                        case 2:
                                            return model.prenom;
                                        case 3:
                                            return model.mail;
                                        default:
                                            return "";
                                    }
                                }
                                selectByMouse: true
                                focus: false
                                enabled: !mouseSection.enabled  // Disable when mouse is active

                                onEditingFinished: {
                                    let id_student = studentManager.mModel.getStudentId(row);
                                    studentManager.updateStudent(id_student, column, text);
                                    sectionField.deselect();
                                    sectionField.focus = false;
                                    mouseSection.enabled = true;
                                }

                                Keys.onEscapePressed: {
                                    focus = false;
                                    mouseSection.enabled = true;
                                }

                                onFocusChanged: {
                                    if (focus) {
                                        mouseSection.enabled = false;
                                    }
                                }
                            }

                            MouseArea {
                                id: mouseSection
                                anchors.fill: parent
                                z: enabled ? 1 : 0  // On top when enabled
                                propagateComposedEvents: true
                                enabled: true  // Start enabled

                                onClicked: mouse => {
                                    if (mouse.modifiers & Qt.ControlModifier) {
                                        model.selected = !model.selected;
                                    } else {
                                        studentManager.mModel.clearSelection();
                                        // Disable self, enable text field
                                        mouseSection.enabled = false;
                                        sectionField.forceActiveFocus();
                                        sectionField.selectAll();
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

