#include "absencemodel.h"
#include <QSqlQuery>
#include <QSqlError>
using am::Etudiant;
AbsenceModel::AbsenceModel(QObject *parent,SeanceModel *s)
    : QAbstractTableModel{parent},m_seance{s}
{


}

int AbsenceModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int AbsenceModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return c_columncount;
}

QVariant AbsenceModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};
    const am::Etudiant &etud = m_data[index.row()];
    switch (role) {
    case InscriRole: return etud.inscri;
    case NomRole: return etud.nom;
    case PrenomRole: return etud.prenom;
    case PresenceRole: return etud.presence;
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.inscri;
        case 1: return etud.nom;
        case 2: return etud.prenom;
        case 3: return etud.presence;
        }
    }
    return {};
}

QHash<int, QByteArray> AbsenceModel::roleNames() const {
    return {
        {InscriRole, "inscri"},{NomRole, "nom"},
        {PrenomRole, "prenom"},
        {PresenceRole, "presence"}
    };
}

bool AbsenceModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    am::Etudiant &e = m_data[index.row()];
    switch (role) {
    case NomRole:
        e.nom = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    case PresenceRole:
        return true;
    default:
        return false;
    }
}

void AbsenceModel::loadEtudiantsForSeance(const int seanceId) {
    qDebug()<<"Received seanceId: "<<seanceId;
    beginResetModel();
    m_data.clear();
    QSqlQuery query;
    query.prepare(R"(
SELECT absence.id,absence.presence_id,etudiant.inscri, etudiant.nom, etudiant.prenom
FROM absence
JOIN etudiant ON absence.etudiant_id = etudiant.id
WHERE absence.seance_id = ? )");

    query.addBindValue(seanceId);

    if (!query.exec()) {
        qWarning() << "Erreur requête absences:" << query.lastError();
        endResetModel();
        return;
    }

    while (query.next()) {
        Etudiant e;
        e.id = query.value("absence.id").toInt();
        e.presence=query.value("absence.presence_id").toInt();
        e.nom = query.value("etudiant.nom").toString();
        e.prenom = query.value("etudiant.prenom").toString();
        e.inscri= query.value("etudiant.inscri").toString();        
        m_data.push_back(e);        
    }

    endResetModel();
    qDebug()<<"Size: "<<m_data.size();
}


bool AbsenceModel::setPresence(int row, int presenceId) {
    if (row < 0 || row >= m_data.size())
        return false;

    m_data[row].presence = presenceId;

    QModelIndex idx = index(row, 3); // colonne 3 = présence
    emit dataChanged(idx, idx, {PresenceRole});


    QSqlQuery query;
    query.prepare("UPDATE absence SET presence_id = ? WHERE id = ?");
    query.addBindValue(presenceId);
    query.addBindValue(m_data[row].id);
    if (!query.exec()) {
        qWarning() << "Erreur UPDATE absence:" << query.lastError();
        return false;
    }

    return true;
}

const QList<Etudiant> &AbsenceModel::getListeEtudiants() const {
    return m_data;
}

SeanceModel *AbsenceModel::getSeance()   const {

    return m_seance;
}

