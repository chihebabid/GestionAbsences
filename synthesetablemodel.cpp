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
    case PourcentageRole: return (etud.nbAbsences/100.0f)*100;
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.inscri;
        case 1: return etud.nom;
        case 2: return etud.prenom;
        case 3: return etud.nbAbsences;
        case 4: return (etud.nbAbsences/100.0f)*100;
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

void SyntheseTableModel::loadAbsencesListForModule(const int idModule) {
    beginResetModel();
    m_data.clear();


    QSqlQuery query (R"(
    SELECT COUNT(*) FROM seance
WHERE module_id = ? AND date <= DATE('now');
    )");
     query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    if (query.next()) {
       setNbSeances(query.value(0).toInt());
    }
    query.clear();
    query.prepare("SELECT COUNT(*) FROM seance WHERE module_id = ? ;");
    query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    if (query.next()) {
        setNbTotalSeances(query.value(0).toInt());
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
