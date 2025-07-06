#include "synthesetablemodel.h"
#include <QSqlQuery>
#include <QSqlError>

SyntheseTableModel::SyntheseTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int SyntheseTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int SyntheseTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return c_columncount;
}

QVariant SyntheseTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};
    const EtudiantAbsence &etud = m_data[index.row()];
    switch (role) {
    case InscriRole: return etud.inscri;
    case NomRole: return etud.nom;
    case PrenomRole: return etud.prenom;
    case NbAbsenceRole: return etud.nbAbsences;
    case PourcentageRole: return (etud.nbAbsences*100.0f)/nbSeances();
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.inscri;
        case 1: return etud.nom;
        case 2: return etud.prenom;
        case 3: return etud.nbAbsences;
        case 4: return (etud.nbAbsences*100.0f)/nbSeances();
        }
    }
    return {};
}

QHash<int, QByteArray> SyntheseTableModel::roleNames() const {
    return {
        {InscriRole, "inscri"},{NomRole, "nom"},
        {PrenomRole, "prenom"},
        {NbAbsenceRole, "presence"},{PourcentageRole,"pourcentage"}
    };
}

void SyntheseTableModel::loadAbsencesListForModule(const int idModule,const QVariantList &typeIds) {
    QString subRequest;


    if (typeIds.isEmpty()) {
        subRequest = ";";
    } else {
        subRequest = " and (";
        for (int i {}; i < typeIds.size(); ++i) {
            subRequest += QString("type_id=%1 ").arg(typeIds[i].toInt());
            if (i < typeIds.size() - 1)
                subRequest += " or ";
        }
        subRequest += ");";
    }

    beginResetModel();
    m_data.clear();
    QSqlQuery query (R"(
    SELECT COUNT(*) FROM seance
WHERE module_id = ? AND date <= DATE('now')
    )"+subRequest);
     query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    if (query.next()) {
       setNbSeances(query.value(0).toInt());
    }
    query.clear();
    query.prepare("SELECT COUNT(*) FROM seance WHERE module_id = ? "+subRequest);
    query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    if (query.next()) {
        setNbTotalSeances(query.value(0).toInt());
    }



    // Charger les absences par étudiant

    query.prepare(R"(
        SELECT e.id,e.inscri, e.nom, e.prenom
            FROM etudiant e
            JOIN section_module sm ON sm.section_id = e.section_id where sm.module_id=?;
    )");
    query.addBindValue(idModule);

    if (!query.exec()) {
        qWarning() << "Erreur chargement etudiants :" << query.lastError();
        endResetModel();
        return;
    }

    while (query.next()) {
        EtudiantAbsence e;
        e.id = query.value("id").toInt();
        e.inscri = query.value("inscri").toString();
        e.nom = query.value("nom").toString();
        e.prenom = query.value("prenom").toString();
        QSqlQuery qAbs;
        qAbs.prepare(R"(
select count(*) from absence ab
join seance s on ab.seance_id=s.id
join presence on ab.presence_id=presence.id
where  ab.etudiant_id=? and (presence.nom!="Présent" and presence.nom!="Retard") and date(s.date)<=date('now')
    )"+subRequest);
        qAbs.addBindValue(e.id);
        if (!qAbs.exec()) {
            qWarning() << "Erreur chargement nbr présences :" << qAbs.lastError();
            endResetModel();
            return;
        }
        if (qAbs.next()) {
            e.nbAbsences=qAbs.value(0).toInt();

        }
        m_data.push_back(e);
    }
    endResetModel();

}

void SyntheseTableModel::setNbSeances(const int nb) {
    m_nb_seances=nb;
}
int SyntheseTableModel::nbSeances() const {
    return m_nb_seances;
}

void SyntheseTableModel::setNbTotalSeances(const int nb) {
    m_nb_total_seances=nb;
}
int SyntheseTableModel::nbTotalSeances() const {
    return m_nb_total_seances;
}
