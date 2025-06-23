#include "modulemodel.h"
#include <QSqlQuery>
#include <QSqlError>

ModuleModel::ModuleModel(QObject *parent)
    : QAbstractListModel{parent}
{}

int ModuleModel::rowCount(const QModelIndex &parent) const {
    return m_modules.size();
}

QVariant ModuleModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_modules.size())
        return {};

    const Module &s = m_modules[index.row()];

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

bool ModuleModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_modules.size())
        return false;

    Module &s = m_modules[index.row()];
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

Qt::ItemFlags ModuleModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ModuleModel::roleNames() const {
    return {
        {ValeurRole, "valeur"},
        {SelectedRole, "selected"}, { NameRole, "name" }
    };
}

void ModuleModel::loadModulesForSection(const int sectionId,const int semestre) {
    qDebug()<<"Section: "<<sectionId<<" Semestre: "<<semestre;
    beginResetModel();
    m_modules.clear();
     QSqlQuery query;
    query.prepare(R"(
        SELECT m.id, m.nom
        FROM module m
        JOIN section_module sm ON sm.module_id = m.id
        WHERE sm.section_id = ? and sm.semestre = ?
    )");

    query.addBindValue(sectionId);
    query.addBindValue(semestre);
    if (!query.exec()) {
        qWarning() << "Erreur requête modules:" << query.lastError();
        endResetModel();
        return;
    }

    while (query.next()) {
        Module m;
        m.id = query.value(0).toInt();
        m.nom = query.value(1).toString();
        m_modules.append(m);
    }

    endResetModel();
}

void ModuleModel::addModuleForSection(const QString &nom,const int sectionId,const int semestre) {
    if (nom.trimmed().isEmpty()) return;
    qDebug()<<"Section: "<<sectionId;
    int moduleId = -1;
    QSqlQuery query;
    query.prepare("SELECT id FROM module WHERE nom = ?");
    query.addBindValue(nom);

    if (query.exec() && query.next()) {
        moduleId = query.value(0).toInt();
    } else {
        // Étape 2 : insérer le module
        query.prepare("INSERT INTO module (nom) VALUES (?)");
        query.addBindValue(nom);

        if (!query.exec()) {
            qWarning() << "Erreur insertion module :" << query.lastError().text();
            return ;
        }
        moduleId = query.lastInsertId().toInt();
    }

    query.prepare(R"(
        INSERT INTO section_module (section_id, module_id, annee_id, semestre)
        VALUES (?, ?, ?, ?)
    )");
    query.addBindValue(sectionId);
    query.addBindValue(moduleId);
    query.addBindValue(2025);
    query.addBindValue(semestre);

    if (!query.exec()) {
        qWarning() << "Erreur insertion section_module :" << query.lastError().text();
        return ;
    }

    loadModulesForSection(sectionId,semestre);
}

void ModuleModel::deleteSelected() {
    QList<int> rowsToDelete;
    QSqlQuery query;

    for (int i = 0; i < m_modules.size(); ++i) {
        if (m_modules[i].selected) {
            query.prepare("DELETE FROM section_module WHERE module_id = ?");
            query.addBindValue(m_modules[i].id);
            if (!query.exec()) {
                qWarning() << "Erreur delete in section_module:" << query.lastError();
            }
            query.prepare("DELETE FROM module WHERE id = ?");
            query.addBindValue(m_modules[i].id);
            if (query.exec()) {
                rowsToDelete.prepend(i); // supprimer en ordre inverse
            } else {
                qWarning() << "Erreur delete:" << query.lastError();
            }
        }
    }

    for (int i : rowsToDelete) {
        beginRemoveRows(QModelIndex(), i, i);
        m_modules.removeAt(i);
        endRemoveRows();
    }
}

void ModuleModel::updateModule(int index, const QString &nouveauNom) {
    if (index < 0 || index >= m_modules.size()) return;
    if (nouveauNom.trimmed().isEmpty()) return;

    Module &s = m_modules[index];

    QSqlQuery query;
    query.prepare("UPDATE module SET nom = ? WHERE id = ?");
    query.addBindValue(nouveauNom);
    query.addBindValue(s.id);

    if (query.exec()) {
        s.nom = nouveauNom;
        emit dataChanged(this->index(index), this->index(index), {ValeurRole});
    } else {
        qWarning() << "Erreur updateModule:" << query.lastError();
    }
}
