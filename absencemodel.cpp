#include "absencemodel.h"


AbsenceModel::AbsenceModel(QObject *parent)
    : QAbstractTableModel{parent}
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

    const Etudiant &etud = m_data[index.row()];
    switch (role) {
    case InscriRole: return etud.inscri;
    case NomRole: return etud.nom;
    case PrenomRole: return etud.prenom;
    case PresenceRole: return "Presence";
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.inscri;
        case 1: return etud.nom;
        case 2: return etud.prenom;
        case 3: return "Presence";
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
