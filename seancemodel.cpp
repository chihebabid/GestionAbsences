#include "seancemodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>


SeanceModel::SeanceModel(QObject *parent) {}

int SeanceModel::rowCount(const QModelIndex &) const {
    return m_data.size();
}

QVariant SeanceModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size())
        return {};

    const Seance &s = m_data[index.row()];

    switch (role) {
    case ValeurRole:
        return QString("%1 - %2 (%3) - %4 de %5 à ").arg(s.section,s.module,s.type,s.date,s.debut);
    default:
        return {};
    }
}


QHash<int, QByteArray> SeanceModel::roleNames() const {
    return {
        {ValeurRole, "valeur"}
    };
}

void SeanceModel::loadSeances() {
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
        Seance s;
        s.section = query.value("section_nom").toString();
        s.module  = query.value("module_nom").toString();
        s.debut   = query.value("seance.heure_debut").toString();
        s.duree   = query.value("seance.duree").toString();
        s.date = query.value("seance.date").toString();
        QDate _date {QDate::fromString(s.date, "yyyy-MM-dd")};
        s.date= _date.toString("dd/MM/yyyy");
        s.type =  query.value("type_cours.libelle").toString();
        s.id= query.value("seance.id").toInt();
        m_data.push_back(s);
    }
    endResetModel();
}

int SeanceModel::getId(const int index) const {
    qDebug()<<"index "<<index<<", data: "<<m_data[index].id;
    return m_data[index].id;
}

const Seance& SeanceModel::getSelectedSeance() const {
    if (m_current_index==-1) qDebug()<<__PRETTY_FUNCTION__;
    return m_data[m_current_index];
}


void SeanceModel::setSelectedIndex(const int index) {
    m_current_index=index;
}
