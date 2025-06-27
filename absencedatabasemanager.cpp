#include "absencedatabasemanager.h"
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "misc.h"

AbsenceDatabaseManager::AbsenceDatabaseManager(QObject *parent)
    : QObject(parent) {}

bool AbsenceDatabaseManager::databaseExists() const {
    if (QFile::exists(am::dbInfo.dbName)) {
        am::dbInfo.db = QSqlDatabase::addDatabase("QSQLITE");
        am::dbInfo.db.setDatabaseName(am::dbInfo.dbName);
        if (!am::dbInfo.db.open()) {
            qWarning() << "Erreur d'ouverture de la base :" << am::dbInfo.db.lastError().text();
            return true;
        }
        return true;
    }
    return false;
}

void AbsenceDatabaseManager::initialize() {
    qDebug()<<__PRETTY_FUNCTION__;
    if (!databaseExists()) {
        qDebug()<<"DB doesn't exist";
        emit askUserToCreateDatabase();
    } else {
        qDebug()<<"DB already exist";
        emit databaseReady();
    }
}

void AbsenceDatabaseManager::createDatabase() {
    am::dbInfo.db = QSqlDatabase::addDatabase("QSQLITE");
    am::dbInfo.db.setDatabaseName(am::dbInfo.dbName);
    if (!am::dbInfo.db.open()) {
        qWarning() << "Erreur d'ouverture de la base :" << am::dbInfo.db.lastError().text();
        return;
    }

    createSchema();
    emit databaseReady();
}

void AbsenceDatabaseManager::createSchema() {
    QSqlQuery query;
    QStringList stmts = {
        // Année universitaire
        R"(CREATE TABLE IF NOT EXISTS annee_universitaire (
            id INTEGER PRIMARY KEY,
            annee TEXT UNIQUE NOT NULL
        ))",

        // Section
        R"(CREATE TABLE IF NOT EXISTS section (
            id INTEGER PRIMARY KEY,
            nom TEXT UNIQUE NOT NULL
        ))",

        // Étudiant
        R"(CREATE TABLE IF NOT EXISTS etudiant (
            id INTEGER PRIMARY KEY,
            nom TEXT NOT NULL,
            prenom TEXT,
            section_id INTEGER NOT NULL,
            FOREIGN KEY (section_id) REFERENCES section(id)
        ))",

        // Enseignant
        R"(CREATE TABLE IF NOT EXISTS enseignant (
            id INTEGER PRIMARY KEY,
            nom TEXT NOT NULL,
            prenom TEXT
        ))",

        // Module
        R"(CREATE TABLE IF NOT EXISTS module (
            id INTEGER PRIMARY KEY,
            nom TEXT NOT NULL
        ))",

        // Association section-module pour une année/un semestre
        R"(CREATE TABLE IF NOT EXISTS section_module (
            id INTEGER PRIMARY KEY,
            section_id INTEGER NOT NULL,
            module_id INTEGER NOT NULL,
            annee_id INTEGER NOT NULL,
            semestre INTEGER NOT NULL CHECK (semestre IN (1, 2)),
            FOREIGN KEY (section_id) REFERENCES section(id),
            FOREIGN KEY (module_id) REFERENCES module(id),
            FOREIGN KEY (annee_id) REFERENCES annee_universitaire(id),
            UNIQUE (section_id, module_id, annee_id, semestre)
        ))",

        // Enseignant associé à un module dans une section donnée
        R"(CREATE TABLE IF NOT EXISTS enseignant_module (
            id INTEGER PRIMARY KEY,
            enseignant_id INTEGER NOT NULL,
            section_module_id INTEGER NOT NULL,
            FOREIGN KEY (enseignant_id) REFERENCES enseignant(id),
            FOREIGN KEY (section_module_id) REFERENCES section_module(id),
            UNIQUE (enseignant_id, section_module_id)
        ))",

        // Absence liée à un étudiant + module enseigné par un enseignant
        R"(CREATE TABLE IF NOT EXISTS absence (
            id INTEGER PRIMARY KEY,
            etudiant_id INTEGER NOT NULL,
            enseignant_module_id INTEGER NOT NULL,
            date_absence TEXT NOT NULL,
            heure_debut TEXT NOT NULL,
            heure_fin TEXT NOT NULL,
            motif TEXT,
            FOREIGN KEY (etudiant_id) REFERENCES etudiant(id),
            FOREIGN KEY (enseignant_module_id) REFERENCES enseignant_module(id)
        ))"
    };

    for (const QString &stmt : stmts) {
        if (!query.exec(stmt)) {
            qWarning() << "Erreur SQL : " << query.lastError().text();
        }
    }
}

