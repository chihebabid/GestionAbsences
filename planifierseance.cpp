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
    query.addBindValue(_date_convertie);         // Format "YYYY-MM-DD"
    query.addBindValue(heureDebut);   // Format "HH:MM"
    query.addBindValue(duree);        // Durée en minutes

    if (!query.exec()) {
        qWarning() << "Erreur lors de l'ajout de la séance:" << query.lastError();
        return false;
    }
    return true;
}
