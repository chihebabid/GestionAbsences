#include "sectionmodel.h"
#include "misc.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SectionModel::SectionModel(QObject *parent)
    : QAbstractListModel(parent)
{
    //openDatabase();
    //loadSections();
}

void SectionModel::openDatabase() {
    am::dbInfo.db = QSqlDatabase::addDatabase("QSQLITE");
    am::dbInfo.db.setDatabaseName(am::dbInfo.dbName);
    if (!am::dbInfo.db.open()) {
        qWarning() << "Erreur ouverture DB:" << am::dbInfo.db.lastError();
    } else {
        QSqlQuery q;
        q.exec("CREATE TABLE IF NOT EXISTS section (id INTEGER PRIMARY KEY, nom TEXT UNIQUE NOT NULL)");
    }
}

int SectionModel::rowCount(const QModelIndex &) const {
    return m_sections.size();
}

QVariant SectionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_sections.size())
        return {};

    const Section &s = m_sections[index.row()];

    switch (role) {
    case ValeurRole:
    case NameRole:
        return s.nom;
    case SelectedRole:
        return s.selected;
    default:
        return {};
    }
}

bool SectionModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_sections.size())
        return false;

    Section &s = m_sections[index.row()];
    switch (role) {
    case ValeurRole:
        s.nom = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    case SelectedRole:
        s.selected = value.toBool();
        emit dataChanged(index, index, {role});
        return true;
    default:
        return false;
    }
}

Qt::ItemFlags SectionModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> SectionModel::roleNames() const {
    return {
        {ValeurRole, "valeur"},
        {SelectedRole, "selected"}, { NameRole, "name" }
    };
}

void SectionModel::loadSections() {
    beginResetModel();
    m_sections.clear();

    QSqlQuery query("SELECT id, nom FROM section");
    while (query.next()) {
        Section s;
        s.id = query.value(0).toInt();
        s.nom = query.value(1).toString();
        s.selected = false;
        m_sections.append(s);
    }

    endResetModel();
}

void SectionModel::insertSection(const QString &nom) {
    if (nom.trimmed().isEmpty()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO section (nom) VALUES (?)");
    query.addBindValue(nom);

    if (!query.exec()) {
        qWarning() << "Erreur insertSection:" << query.lastError();
        return;
    }

    loadSections();
}

void SectionModel::updateSection(int index, const QString &nouveauNom) {
    if (index < 0 || index >= m_sections.size()) return;
    if (nouveauNom.trimmed().isEmpty()) return;

    Section &s = m_sections[index];

    QSqlQuery query;
    query.prepare("UPDATE section SET nom = ? WHERE id = ?");
    query.addBindValue(nouveauNom);
    query.addBindValue(s.id);

    if (query.exec()) {
        s.nom = nouveauNom;
        emit dataChanged(this->index(index), this->index(index), {ValeurRole});
    } else {
        qWarning() << "Erreur updateSection:" << query.lastError();
    }
}

void SectionModel::deleteSelected() {
    QList<int> rowsToDelete;
    QSqlQuery query;

    for (int i = 0; i < m_sections.size(); ++i) {
        if (m_sections[i].selected) {
            query.prepare("DELETE FROM section WHERE id = ?");
            query.addBindValue(m_sections[i].id);
            if (query.exec()) {
                rowsToDelete.prepend(i); // supprimer en ordre inverse
            } else {
                qWarning() << "Erreur delete:" << query.lastError();
            }
        }
    }

    for (int i : rowsToDelete) {
        beginRemoveRows(QModelIndex(), i, i);
        m_sections.removeAt(i);
        endRemoveRows();
    }
}
void SectionModel::clearSelection() {

    for(size_t i{};i<m_sections.size();++i) {
        m_sections[i].selected=false;
         emit dataChanged(index(i), index(i), {SelectedRole});
    }
}


int SectionModel::getSectionId(int indexModel) const {
    if (indexModel < 0 || indexModel >= m_sections.size())
        return -1;
    qDebug()<<"IndexModel: "<<indexModel;
    qDebug()<<"Id: "<<m_sections[indexModel].id;
    return m_sections[indexModel].id;
}
