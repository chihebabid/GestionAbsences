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
    case HasSeancesRole: return tc.hasSeances;
    default: return QVariant();
    }
}

QHash<int, QByteArray> TypeCoursModel::roleNames() const
{
    return {
        { IdRole, "id" },
        { NomRole, "nom" },
        { HasSeancesRole, "hasSeances" }
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

    QSqlQuery query("SELECT id, libelle FROM type_cours");
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

void TypeCoursModel::loadTypesCoursForModule(const int idModule) {
    beginResetModel();
    m_data.clear();

    // Charger tous les types
    QSqlQuery query("SELECT id, libelle FROM type_cours");
    while (query.next()) {
        TypeCours t;
        t.id = query.value(0).toInt();
        t.nom = query.value(1).toString();

        // Vérifie s'il y a des séances planifiées pour ce type dans le module donné
        QSqlQuery q;
        q.prepare("SELECT COUNT(*) FROM seance WHERE module_id = :mid AND type_id = :tid");
        q.bindValue(":mid", idModule);
        q.bindValue(":tid", t.id);
        q.exec();
        if (q.next()) {
            t.hasSeances = q.value(0).toInt() > 0;
        }

        m_data.append(t);
    }
    endResetModel();
}
