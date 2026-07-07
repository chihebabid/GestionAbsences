#include "coursetypemodel.h"
#include <QSqlQuery>
#include <QSqlError>

CourseTypeModel::CourseTypeModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int CourseTypeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

QVariant CourseTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_data.size())
        return QVariant();

    const CourseType &tc = m_data[index.row()];
    switch (role) {
    case IdRole: return tc.id;
    case NameRole: return tc.name;
    case HasSessionsRole: return tc.hasSessions;
    default: return QVariant();
    }
}

QHash<int, QByteArray> CourseTypeModel::roleNames() const
{
    return {
        { IdRole, "id" },
        { NameRole, "name" },
        { HasSessionsRole, "hasSessions" }
    };
}

int CourseTypeModel::getId(int index) const
{
    return (index >= 0 && index < m_data.size()) ? m_data[index].id : -1;
}

QString CourseTypeModel::getName(int index) const
{
    return (index >= 0 && index < m_data.size()) ? m_data[index].name : QString();
}

void CourseTypeModel::load()
{
    beginResetModel();
    m_data.clear();

    QSqlQuery query("SELECT id, libelle FROM type_cours");
    while (query.next()) {
        m_data.append(CourseType {
            query.value(0).toInt(),
            query.value(1).toString(),
            false
        });
    }

    if (query.lastError().isValid()) {
        qWarning() << "Erreur de lecture type_cours:" << query.lastError();
    }

    endResetModel();
}

void CourseTypeModel::loadCourseTypesForModule(const int idModule) {
    beginResetModel();
    m_data.clear();

    // Charger tous les types
    QSqlQuery query("SELECT id, libelle FROM type_cours");
    while (query.next()) {
        CourseType t;
        t.id = query.value(0).toInt();
        t.name = query.value(1).toString();

        // Vérifie s'il y a des séances planifiées pour ce type dans le module donné
        QSqlQuery q;
        q.prepare("SELECT COUNT(*) FROM seance WHERE module_id = :mid AND type_id = :tid");
        q.bindValue(":mid", idModule);
        q.bindValue(":tid", t.id);
        q.exec();
        if (q.next()) {
            t.hasSessions = q.value(0).toInt() > 0;
        }

        m_data.append(t);
    }
    endResetModel();
}


