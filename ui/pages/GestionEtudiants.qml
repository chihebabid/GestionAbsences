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


    MyText {
        text: "Année universitaire : " + (databaseManager.educationYear)[0] + " - " + (databaseManager.educationYear)[1]
        font.pixelSize: 18
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }
    MyGroupBox {
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        title: qsTr("Gérer les étudiants")
        RowLayout {
            anchors.fill: parent
            implicitWidth: parent.width
            anchors.margins: 0
            spacing: 6

            ColumnLayout {
                id: myColumnEtudiants
                Layout.fillWidth: true
                Layout.preferredWidth: parent.width * 0.85 // 85% pour la colonne gauche
                Layout.fillHeight: true // Remplit la hauteur
                anchors.margins: 0
                spacing: 1
                property var columnWidths: [myColumnEtudiants.width * .2, myColumnEtudiants.width * .3, myColumnEtudiants.width * .3, myColumnEtudiants.width * .2 - 4]
                Row {
                    spacing: 5
                    anchors.margins: 5
                    MyText {
                        text: "Section :"
                    }

                    MyComboBox {
                        id: sectionCombo
                        textRole: "name"
                        width: root.width * 0.3
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

                    Item {
                        Layout.fillHeight: true // Absorbe tout l'espace restant en bas
                    }
                }

                Item {
                    height: 6
                }

                // Tableau
                Row {
                    id: header
                    Layout.fillWidth: true
                    width: myColumnEtudiants.width
                    height: 40
                    spacing: 1

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
                    width: myColumnEtudiants.width
                    height: 300
                    property bool toRevert: false
                    model: studentManager.mModel
                    leftMargin: 0
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
            Column {
                spacing: 4
                Layout.fillWidth: true // Prend le reste de la largeur (15%)
                Layout.fillHeight: true // Remplit la hauteur pour éviter le centrage
                Item {
                    height: 80
                    width: 1
                }

                MyButton {
                    id: bSupprimer
                    text: "Supprimer"
                    onClicked: {

                        // moduleModel.deleteSelected()
                    }
                }
                MyButton {
                    id: bImporter
                    text: "Importer"
                    enabled: false
                    onClicked: {
                        fileDialog.open();
                    }
                }
            }
        }
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
    }

}

