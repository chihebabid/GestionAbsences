#include "planifierseance.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
PlanifierSeance::PlanifierSeance(QObject *parent)
    : QObject{parent}
{}


bool PlanifierSeance::ajouterSeance(const int moduleId, const int typeCours, const QString& date, const QString& heureDebut, const int duree) {
    // Convertir la date au format YYYY-MM-DD
    QDate _date {QDate::fromString(date, "dd/MM/yyyy")};
    QString _date_convertie {_date.toString("yyyy-MM-dd")};

    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO seance (module_id, type_id, date, heure_debut, duree)
        VALUES (?, ?, ?, ?, ?)
    )");

    query.addBindValue(moduleId);
    query.addBindValue(typeCours);
    query.addBindValue(_date_convertie);
    query.addBindValue(heureDebut);
    query.addBindValue(duree);

    if (!query.exec()) {
        qWarning() << "Erreur lors de l'ajout de la séance:" << query.lastError();
        qWarning()<<"module_id: "<<moduleId<<" type_id:"<<typeCours<<" date: "<<_date_convertie<<" duree: "<<duree;
        return false;
    }

    // Récupérer l'identifiant de la séance créée
    int seanceId = query.lastInsertId().toInt();

    // Étape 1 : Trouver la section associée au module via section_module
    int sectionId = -1;
    QSqlQuery sectionQuery;
    sectionQuery.prepare(R"(
        SELECT section_id
        FROM section_module
        WHERE module_id = ?
        LIMIT 1
    )");
    sectionQuery.addBindValue(moduleId);

    if (sectionQuery.exec() && sectionQuery.next()) {
        sectionId = sectionQuery.value(0).toInt();
    } else {
        qWarning() << "Impossible de trouver la section associée au module:" << sectionQuery.lastError();
        return false;
    }

    // Étape 2 : Récupérer les étudiants de cette section
    QSqlQuery etudiantQuery;
    etudiantQuery.prepare(R"(
        SELECT id
        FROM etudiant
        WHERE section_id = ?
    )");
    etudiantQuery.addBindValue(sectionId);

    if (!etudiantQuery.exec()) {
        qWarning() << "Erreur lors de la récupération des étudiants:" << etudiantQuery.lastError();
        return false;
    }

    // Étape 3 : Insérer une ligne dans absence pour chaque étudiant
    QSqlQuery insertAbsence;


    const int defaultPresenceId = 1; // Exemple: 1 = "Non renseigné"
    while (etudiantQuery.next()) {
        int etudiantId = etudiantQuery.value(0).toInt();
        insertAbsence.prepare(R"(
        INSERT INTO absence (seance_id, etudiant_id, presence_id, date)
        VALUES (?, ?, ?, ?)
        )");
        insertAbsence.addBindValue(seanceId);
        insertAbsence.addBindValue(etudiantId);
        insertAbsence.addBindValue(defaultPresenceId);
        insertAbsence.addBindValue(_date_convertie);

        if (!insertAbsence.exec()) {
            qWarning() << "Erreur lors de l'ajout d'une absence pour l'étudiant ID" << etudiantId << ":" << insertAbsence.lastError();
            // Continuer ou return false selon la politique
        }

    }

    return true;
}

