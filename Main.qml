import QtQuick

import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Fusion

import "."

ApplicationWindow {
    id: root
    //Material.theme: Material.Dark
    visible: true
    width: 640
    height: 480
    title: "Gestion des absences"
    property bool isDatabaseReady: false
    property int currentTab: 0

    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            currentIndex: currentTab
            onCurrentIndexChanged: currentTab = currentIndex

            TabButton { text: "Accueil" }
            TabButton { text: "Gestion des Sections" }
            TabButton { text: "Gestion des Modules" }
            TabButton { text: "Gestion des Étudiants" }
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
               root.isDatabaseReady = true;
               console.log("Base de données prête !");
        }
    }
}
