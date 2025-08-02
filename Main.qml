import QtQuick

import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material

import "."

ApplicationWindow {
    id: root
    //Material.theme: Material.Dark
    visible: true
    width: 800
    height: 600
    title: "Gestion des absences"
    property bool isDatabaseReady: false
    property int currentTab: 0

    AboutDialog {
            id: aboutDialog

    }

    ColumnLayout {
        anchors.fill: parent

        PageDrawer {
            id: mainDrawer
            // Icon properties
            iconTitle: "Gestion des absences"
            iconSource: "qrc:/icons/enhance.png"
            iconSubtitle: qsTr("Version 1.0.0 Alpha")
            // Define the actions to take for each drawer item
            // Drawers 5 and 6 are ignored, because they are used for
            // displaying a spacer and a separator
            actions: {
                "0": function () {
                    console.log("Item 1 clicked!")
                },
                "1": function () {
                    console.log("Item 2 clicked!")
                },
                "2": function () {
                    mainDrawer.close()
                    aboutDialog.open()
                },
                "3": function () {
                    mainDrawer.close()
                    Qt.quit()
                },
                "4": function () {
                    Qt.quit()
                }
            }
            // Define the drawer items
            items: ListModel {
                id: pagesModel

                ListElement {
                    pageTitle: qsTr("Ouvrir")
                    pageIcon: "qrc:/icons/blue-fileopen.svg"
                }

                ListElement {
                    pageTitle: qsTr("Fermer")
                    pageIcon: "qrc:/icons/help-about.svg"
                }

                ListElement {
                    pageTitle: qsTr("À propos")
                    pageIcon: "qrc:/icons/help-about.svg"
                }

                //    ListElement {
                //                     spacer: true
                //                   }

                //                     ListElement {
                //                          separator: true
                //                       }
                ListElement {
                    pageTitle: qsTr("Quitter")
                    pageIcon: "qrc:/icons/application-exit.svg"
                }
            }
        }

        ToolBar {
            Layout.fillWidth: true
            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "#0aabff"
                    }
                    GradientStop {
                        position: 1.0
                        color: "#FFFBFE"
                    }
                }
            }
            RowLayout {
                spacing: 10

                Rectangle {
                    width: 40  // Adjust size as needed
                    height: 40
                    color: mouseArea.containsPress ? Qt.darker(palette.highlight, 1.2) :
                           mouseArea.containsMouse ? palette.highlight : "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: "\u2630" // Hamburger icon unicode
                        font.pixelSize: 16
                        color: palette.buttonText // Use system text color for contrast
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: mainDrawer.open()
                    }
                }
            }
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            currentIndex: currentTab
            onCurrentIndexChanged: currentTab = currentIndex

            TabButton {
                text: "Accueil"
            }
            TabButton {
                text: "Synthèse"
            }
            TabButton {
                text: "Gestion des Sections"
            }
            TabButton {
                text: "Gestion des Modules"
            }
            TabButton {
                text: "Gestion des Étudiants"
            }
        }

        StackLayout {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: currentTab

            // Onglet 0
            Loader {
                source: isDatabaseReady ? "WelcomeInterface.qml" : ""
            }

            Loader {
                source: isDatabaseReady ? "Synthese.qml" : ""
            }

            // Onglet 1
            Loader {
                source: isDatabaseReady ? "GestionSections.qml" : ""
            }

            // Onglet 2
            Loader {
                source: isDatabaseReady ? "GestionModules.qml" : ""
            }

            // Onglet 2
            Loader {
                source: isDatabaseReady ? "GestionEtudiants.qml" : ""
            }
        }
    }

    MessageDialog {
        id: confirmDialog
        title: "Initialisation de la base de données"
        informativeText: "La base de données des absences est introuvable.\nSouhaitez-vous la créer ?"
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: databaseManager.createDatabase()
    }

    Connections {
        target: databaseManager
        function onAskUserToCreateDatabase() {
            confirmDialog.open()
        }

        function onDatabaseReady() {
            root.isDatabaseReady = true
            console.log("Base de données prête !")
        }
    }
}
