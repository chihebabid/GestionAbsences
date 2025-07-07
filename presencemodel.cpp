#include "presencemodel.h"
#include <QSqlQuery>



PresenceModel::PresenceModel(QObject *parent): QAbstractListModel{parent} {

}

void PresenceModel::loadFromDatabase() {
    QSqlQuery query("SELECT id, nom FROM presence");
    while (query.next()) {
        m_data.append({query.value(0).toInt(), query.value(1).toString()});
    }
}

int PresenceModel::rowCount(const QModelIndex&) const {
    return m_data.size();
}

QVariant PresenceModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size())
        return {};
    const auto& p = m_data[index.row()];
    if (role == Qt::DisplayRole || role==LabelRole) return p.label;
    if (role==IdRole) return p.id;
    return {};
}

QHash<int, QByteArray> PresenceModel::roleNames() const {
    return {{Qt::DisplayRole, "label"},{IdRole,"id"}};
}

int PresenceModel::getId(int index) const {
    return (index >= 0 && index < m_data.size()) ? m_data[index].id : -1;
}

QString PresenceModel::getLabel(int index) const {
    return (index >= 0 && index < m_data.size()) ? m_data[index].label : QString{};
}

int PresenceModel::indexOfLabel(const QString& label) const {
    for (int i = 0; i < m_data.size(); ++i)
        if (m_data[i].label == label)
            return i;
    return -1;
}


int PresenceModel::getIndexById(int id) const {

    for (int i {}; i < m_data.size(); ++i)
        if (m_data[i].id == id)
            return i;
    return -1;
}
