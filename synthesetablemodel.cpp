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
    case PourcentageRole: return nbSeances() ? (etud.nbAbsences*100.0f)/nbSeances() : 0;
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.inscri;
        case 1: return etud.nom;
        case 2: return etud.prenom;
        case 3: return etud.nbAbsences;
        case 4: return nbSeances() ? (etud.nbAbsences*100.0f)/nbSeances() : 0;
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

    beginResetModel();
    m_data.clear();
    m_types_module.clear();
    if (typeIds.isEmpty()) {
        setNbSeances(0);
        setNbTotalSeances(0);
        endResetModel();
        return ;
    } else {
        subRequest = " and (";
        for (int i {}; i < typeIds.size(); ++i) {
            subRequest += QString("type_id=%1 ").arg(typeIds[i].toInt());
            if (i < typeIds.size() - 1)
                subRequest += " or ";
        }
        subRequest += ");";
    }
    {
        // Remplir ml_types_module
        QString _request;
        for (int i {}; i < typeIds.size(); ++i) {
            _request += QString("id=%1 ").arg(typeIds[i].toInt());
            if (i < typeIds.size() - 1)
                _request += " or ";
        }
        QSqlQuery query {R"(select libelle from type_cours where )"+_request};
        if (!query.exec()) {
            qWarning() << "Erreur requête type cours :" << query.lastError();
            return;
        }

        QStringList libelles;
        while (query.next()) {
            libelles << query.value(0).toString();
        }

        // Joindre les libellés avec une virgule et un espace
        m_types_module = libelles.join(", ");



    }

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
        if (m_nb_seances) {
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
        }

        m_data.push_back(e);
    }

    // Charger les informations utiles : nom de la section, module, etc.
    query.clear();
    query.prepare(R"(
select sm.semestre,se.nom,mo.nom from section_module sm
join section se on sm.module_id=se.id
join module mo on sm.module_id=mo.id
where sm.module_id=? )");
    query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur chargement infos :" << query.lastError();
        endResetModel();
        return;
    }
    if (query.next()) {
        m_info.semestre = query.value(0).toInt();
        m_info.section = query.value(1).toString();
        m_info.module = query.value(2).toString();

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

am::Info_t SyntheseTableModel::getCurrentInfo() const {
    return m_info;
}

const QList<EtudiantAbsence> &SyntheseTableModel::getListeEtudiants() const {
    return m_data;
}

const QString SyntheseTableModel::getListeTypesModule() const {
    return m_types_module;
}


