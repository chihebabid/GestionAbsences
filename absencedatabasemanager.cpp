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
            annee TEXT NOT NULL UNIQUE
        ))",

        // Section
        R"(CREATE TABLE IF NOT EXISTS section (
            id INTEGER PRIMARY KEY,
            nom TEXT NOT NULL UNIQUE
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

        // Section_Module
        R"(CREATE TABLE IF NOT EXISTS section_module (
            id INTEGER PRIMARY KEY,
            section_id INTEGER NOT NULL,
            module_id INTEGER NOT NULL,
            annee_id INTEGER NOT NULL,
            semestre INTEGER NOT NULL CHECK(semestre IN (1, 2)),
            FOREIGN KEY(section_id) REFERENCES section(id),
            FOREIGN KEY(module_id) REFERENCES module(id),
            FOREIGN KEY(annee_id) REFERENCES annee_universitaire(id),
            UNIQUE(section_id, module_id, annee_id, semestre)
        ))",

        // Enseignant_Module
        R"(CREATE TABLE IF NOT EXISTS enseignant_module (
            id INTEGER PRIMARY KEY,
            enseignant_id INTEGER NOT NULL,
            section_module_id INTEGER NOT NULL,
            FOREIGN KEY(enseignant_id) REFERENCES enseignant(id),
            FOREIGN KEY(section_module_id) REFERENCES section_module(id),
            UNIQUE(enseignant_id, section_module_id)
        ))",

        // Étudiant
        R"(CREATE TABLE IF NOT EXISTS etudiant (
            id INTEGER PRIMARY KEY,
            inscri TEXT NOT NULL UNIQUE,
            nom TEXT NOT NULL,
            prenom TEXT,
            mail TEXT NOT NULL,
            section_id INTEGER NOT NULL,
            FOREIGN KEY(section_id) REFERENCES section(id)
        ))",

        // Type de cours
        R"(CREATE TABLE IF NOT EXISTS type_cours (
            id INTEGER PRIMARY KEY,
            libelle TEXT UNIQUE
        ))",

        // Séance
        R"(CREATE TABLE IF NOT EXISTS seance (
            id INTEGER PRIMARY KEY,
            module_id INTEGER NOT NULL,
            type_id INTEGER NOT NULL,
            date TEXT NOT NULL,
            heure_debut TEXT NOT NULL,
            duree INTEGER NOT NULL,
            FOREIGN KEY(module_id) REFERENCES module(id),
            FOREIGN KEY(type_id) REFERENCES type_cours(id)
        ))",

        // Présence
        R"(CREATE TABLE IF NOT EXISTS presence (
            id INTEGER PRIMARY KEY,
            nom TEXT NOT NULL
        ))",

        // Absence
        R"(CREATE TABLE IF NOT EXISTS absence (
            id INTEGER PRIMARY KEY,
            seance_id INTEGER NOT NULL,
            etudiant_id INTEGER NOT NULL,
            presence_id INTEGER NOT NULL,
            date TEXT NOT NULL,
            FOREIGN KEY(seance_id) REFERENCES seance(id),
            FOREIGN KEY(etudiant_id) REFERENCES etudiant(id),
            FOREIGN KEY(presence_id) REFERENCES presence(id)
        ))",

        // INSERTS pour type_cours
        R"(INSERT OR IGNORE INTO type_cours (id, libelle) VALUES
            (1, 'Cours'),
            (2, 'TD'),
            (3, 'TP'),
            (4, 'CI')
        )",

        // INSERTS pour presence
        R"(INSERT OR IGNORE INTO presence (id, nom) VALUES
            (1, ''),
            (2, 'Présent'),
            (3, 'Absent'),
            (4, 'Retard')
        )"
    };

    for (const QString &stmt : stmts) {
        if (!query.exec(stmt)) {
            qWarning() << "Erreur SQL : " << query.lastError().text();
        }
    }
}


