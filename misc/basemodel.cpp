#include "basemodel.h"

BaseModel::BaseModel(QObject *parent) : QAbstractTableModel(parent) {

}


int BaseModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int BaseModel::columnCount(const QModelIndex &parent) const  {
    return _columnCount;
}

void BaseModel::reset() {
    beginResetModel();
    endResetModel();
}


void BaseModel::set(const std::vector<std::shared_ptr<base_struct_t>> &data) {
    beginResetModel();
    m_data = data;
    endResetModel();
}
