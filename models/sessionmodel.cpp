#include "misc.h"
#include "sessionmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>


SessionModel::SessionModel(QObject *parent) {}

int SessionModel::rowCount(const QModelIndex &) const {
    return m_data.size();
}

QVariant SessionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size())
        return {};

    const Session &s = m_data[index.row()];

    switch (role) {
    case ValueRole:
        return QString("%1 - %2 (%3) - %4 from %5 to %6").arg(s.section,s.module,s.type,s.date,s.start,am::calculateEndTime(s.start,s.duration.toInt()));
    default:
        return {};
    }
}


QHash<int, QByteArray> SessionModel::roleNames() const {
    return {
        {ValueRole, "value"}
    };
}

void SessionModel::loadSessions() {
    beginResetModel();
    m_data.clear();


    QSqlQuery query (R"(
    SELECT
    section.nom AS section_nom,
    module.nom AS module_nom,
    seance.date,
    seance.heure_debut,
    seance.duree,
    seance.id,
    type_cours.libelle
    FROM seance
    JOIN type_cours ON seance.type_id = type_cours.id
    JOIN module ON seance.module_id = module.id
    JOIN section_module ON section_module.module_id = module.id
    JOIN section ON section.id = section_module.section_id
    WHERE seance.date >= date('now')
    ORDER BY seance.date ASC;
    )");

    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    while (query.next()) {
        Session s;
        s.section = query.value("section_nom").toString();
        s.module  = query.value("module_nom").toString();
        s.start   = query.value("seance.heure_debut").toString();
        s.duration   = query.value("seance.duree").toString();
        s.date = query.value("seance.date").toString();
        QDate _date {QDate::fromString(s.date, "yyyy-MM-dd")};
        s.date= _date.toString("dd/MM/yyyy");
        s.type =  query.value("type_cours.libelle").toString();
        s.id= query.value("seance.id").toInt();
        m_data.push_back(s);
    }
    endResetModel();
}

int SessionModel::getId(const int index) const {
    return m_data[index].id;
}

const Session& SessionModel::getSelectedSession() const {
    return m_data[m_current_index];
}


void SessionModel::setSelectedIndex(const int index) {
    m_current_index=index;
}


