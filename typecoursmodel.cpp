#include "typecoursmodel.h"
#include <QSqlQuery>
#include <QSqlError>

TypeCoursModel::TypeCoursModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int TypeCoursModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

QVariant TypeCoursModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_data.size())
        return QVariant();

    const TypeCours &tc = m_data[index.row()];
    switch (role) {
    case IdRole: return tc.id;
    case NomRole: return tc.nom;
    default: return QVariant();
    }
}

QHash<int, QByteArray> TypeCoursModel::roleNames() const
{
    return {
        { IdRole, "id" },
        { NomRole, "nom" }
    };
}

int TypeCoursModel::getId(int index) const
{
    return (index >= 0 && index < m_data.size()) ? m_data[index].id : -1;
}

QString TypeCoursModel::getNom(int index) const
{
    return (index >= 0 && index < m_data.size()) ? m_data[index].nom : QString();
}

void TypeCoursModel::load()
{
    beginResetModel();
    m_data.clear();

    QSqlQuery query("SELECT id, libelle FROM type_cours ORDER BY libelle ASC");
    while (query.next()) {
        m_data.append(TypeCours {
            query.value(0).toInt(),
            query.value(1).toString()
        });
    }

    if (query.lastError().isValid()) {
        qWarning() << "Erreur de lecture type_cours:" << query.lastError();
    }

    endResetModel();
}
