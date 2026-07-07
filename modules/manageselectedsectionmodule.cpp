#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "manageselectedsectionmodule.h"
#include "misc.h"

ManageSelectedSectionModule::ManageSelectedSectionModule(QObject *parent)
    : QObject{parent}
{
    /*m_sectionsModel = new QStringListModel(this);

    // Ouvrez la base de données SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(am::dbName);

    if (!db.open()) {
        qWarning() << "Erreur: Impossible d'ouvrir la base de données";
        return;
    }

    loadSectionsFromDatabase();*/
}

void ManageSelectedSectionModule::loadSectionsFromDatabase() {
    QStringList sections;

    QSqlQuery query("SELECT nom_section FROM sections"); // Adaptez à votre table
    while (query.next()) {
        sections << query.value(0).toString();
    }

    m_sectionsModel->setStringList(sections);
    emit sectionsModelChanged(); // Notifie QML du changement
}

bool ManageSelectedSectionModule::insertSection(QString v) {
    QSqlQuery query;
    if (!query.exec("INSERT INTO section (nom) VALUES ('"+v+ "')")) {
        qWarning() << "Erreur SQL :" << query.lastError().text();
    }
    return true;
}


