#include "summarytablemodel.h"
#include <QSqlQuery>
#include <QSqlError>

SummaryTableModel::SummaryTableModel(QObject *parent)
    : QAbstractTableModel{parent} {
}

int SummaryTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int SummaryTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return c_columncount;
}

QVariant SummaryTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};
    const StudentAbsence &etud = m_data[index.row()];
    switch (role) {
        case InscriRole: return etud.inscri;
        case LastNameRole: return etud.lastName;
        case FirstNameRole: return etud.firstName;
        case NbAbsenceRole: return etud.nbAbsences;
        case PercentageRole: return nbSessions() ? (etud.nbAbsences * 100.0f) / nbSessions() : 0;
        case Qt::DisplayRole:
            switch (index.column()) {
                case 0: return etud.inscri;
                case 1: return etud.lastName;
                case 2: return etud.firstName;
                case 3: return etud.nbAbsences;
                case 4: return nbSessions() ? (etud.nbAbsences * 100.0f) / nbSessions() : 0;
            }
    }
    return {};
}

QHash<int, QByteArray> SummaryTableModel::roleNames() const {
    return {
        {InscriRole, "inscri"}, {LastNameRole, "lastName"},
        {FirstNameRole, "firstName"},
        {NbAbsenceRole, "presence"}, {PercentageRole, "percentage"}
    };
}

void SummaryTableModel::loadAbsencesListForModule(const int idModule, const QVariantList &typeIds) {
    QString subRequest;

    beginResetModel();
    m_data.clear();
    m_types_course.clear();
    if (typeIds.isEmpty()) {
        setNbSessions(0);
        setNbTotalSessions(0);
        endResetModel();
        return;
    } else {
        subRequest = " and (";
        for (int i{}; i < typeIds.size(); ++i) {
            subRequest += QString("type_id=%1 ").arg(typeIds[i].toInt());
            if (i < typeIds.size() - 1)
                subRequest += " or ";
        }
        subRequest += ");";
    } {
        // Fill m_types_course
        QString _request;
        for (int i{}; i < typeIds.size(); ++i) {
            _request += QString("id=%1 ").arg(typeIds[i].toInt());
            if (i < typeIds.size() - 1)
                _request += " or ";
        }
        QSqlQuery query{R"(select libelle from type_cours where )" + _request};
        if (!query.exec()) {
            qWarning() << "Erreur requête type cours :" << query.lastError();
            return;
        }

        QStringList libelles;
        while (query.next()) {
            libelles << query.value(0).toString();
        }

        m_types_course = libelles.join(", ");
    }

    QSqlQuery query(R"(
                    SELECT COUNT(*) FROM seance
                WHERE module_id = ? AND date <= DATE('now')
                    )" + subRequest);
    query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    if (query.next()) {
        setNbSessions(query.value(0).toInt());
    }
    query.clear();
    query.prepare("SELECT COUNT(*) FROM seance WHERE module_id = ? " + subRequest);
    query.addBindValue(idModule);
    if (!query.exec()) {
        qWarning() << "Erreur requête séances :" << query.lastError();
        return;
    }

    if (query.next()) {
        setNbTotalSessions(query.value(0).toInt());
    }

    // Load absences per student

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
        StudentAbsence e;
        e.id = query.value("id").toInt();
        e.inscri = query.value("inscri").toString();
        e.lastName = query.value("nom").toString();
        e.firstName = query.value("prenom").toString();
        if (m_nb_sessions) {
            QSqlQuery qAbs;
            qAbs.prepare(R"(
                select count(*) from absence ab
                join seance s on ab.seance_id=s.id
                join presence on ab.presence_id=presence.id
                where  ab.etudiant_id=? and (presence.nom!="Présent" and presence.nom!="Retard") and date(s.date)<=date('now')
                    )" + subRequest);
            qAbs.addBindValue(e.id);
            if (!qAbs.exec()) {
                qWarning() << "Erreur chargement nbr présences :" << qAbs.lastError();
                endResetModel();
                return;
            }
            if (qAbs.next()) {
                e.nbAbsences = qAbs.value(0).toInt();
            }
        }

        m_data.push_back(e);
    }

    // Load info: section name, module, etc.
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

void SummaryTableModel::setNbSessions(const int nb) {
    m_nb_sessions = nb;
}

int SummaryTableModel::nbSessions() const {
    return m_nb_sessions;
}

void SummaryTableModel::setNbTotalSessions(const int nb) {
    m_nb_total_sessions = nb;
}

int SummaryTableModel::nbTotalSessions() const {
    return m_nb_total_sessions;
}

am::Info_t SummaryTableModel::getCurrentInfo() const {
    return m_info;
}

const QList<StudentAbsence> &SummaryTableModel::getStudentList() const {
    return m_data;
}

const QString SummaryTableModel::getCourseTypeList() const {
    return m_types_course;
}

