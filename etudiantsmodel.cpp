#include "etudiantsmodel.h"
#include <QSqlError>
#include <QSqlQuery>

EtudiantsModel::EtudiantsModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    m_data.push_back({"ok", "ok", "kjslksj"});
    m_data.push_back({"ok", "ok", "kjslksj"});
    m_data.push_back({"ok", "ok", "kjslksj"});

}

int EtudiantsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int EtudiantsModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return c_columncount;
}

QVariant EtudiantsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};

    const Etudiant &etud = m_data[index.row()];
    switch (role) {
    case NomRole: return etud.nom;
    case PrenomRole: return etud.prenom;
    case MailRole: return etud.mail;
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.nom;
        case 1: return etud.prenom;
        case 2: return etud.mail;
        }
    }
    return {};
}

QVariant EtudiantsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Nom";
        case 1: return "Prénom";
        case 2: return "Mail";
        }
    }
    return {};
}
QHash<int, QByteArray> EtudiantsModel::roleNames() const {
    return {
        {NomRole, "nom"},
        {PrenomRole, "prenom"},
        {MailRole, "mail"}
    };
}

void EtudiantsModel::loadEtudiantsForSection(const int sectionId) {

    qDebug()<<__PRETTY_FUNCTION__<<", section id : "<<sectionId;
    if (!QSqlDatabase::database().isOpen()) {
        qWarning() << "Base de données non ouverte !";
    }
    beginResetModel();
    m_data.clear();
    QSqlQuery query;
    query.prepare(R"(
        SELECT nom, prenom
        FROM etudiant
        WHERE section_id = ? )");

    query.addBindValue(sectionId);

    if (!query.exec()) {
        qWarning() << "Erreur requête modules:" << query.lastError();
        endResetModel();
        return;
    }

    while (query.next()) {
        Etudiant e;
        e.nom = query.value(0).toString();
        e.prenom = query.value(1).toString();
        m_data.push_back(e);
        qDebug()<<"Itération";
    }

    endResetModel();
    qDebug()<<"Nombre etudiants : "<<m_data.count();
}


bool EtudiantsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    Etudiant &e = m_data[index.row()];
    switch (role) {
    case NomRole:
        e.nom = value.toString();
        emit dataChanged(index, index, {role});
        return true;

    default:
        return false;
    }
}
