#include "studentmodel.h"

StudentModel::StudentModel(QObject *parent) : BaseModel{parent} {
    _columnCount=4;
}

QVariant StudentModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_data.size()) {
        qDebug() << __PRETTY_FUNCTION__<<" - Invalid index: " << index.row();
        return {};
    }
    const auto &student = std::dynamic_pointer_cast<student_t>(m_data[index.row()]);
    switch (role) {
        case InscriRole: return student->inscri;
        case NomRole: return student->name;
        case PrenomRole: return student->firstName;
        case MailRole: return student->mail;
        case SelectRole: return student->selected;
        case Qt::DisplayRole:
        switch (index.column()) {
            case 0: return student->inscri;
            case 1: return student->name;
            case 2: return student->firstName;
            case 3: return student->mail;
            case 4: return student->selected;
        }
    }
    return {};
}

QHash<int, QByteArray> StudentModel::roleNames() const {
    return {
        {InscriRole, "inscri"},{NomRole, "nom"},
        {PrenomRole, "prenom"},
        {MailRole, "mail"},{SelectRole,"selected"}
    };
}

Qt::ItemFlags StudentModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool StudentModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    const auto &student {std::dynamic_pointer_cast<student_t>(m_data[index.row()])};
    const auto topLeft {index.sibling(index.row(), 0)};
    const auto bottomRight {index.sibling(index.row(), columnCount(index) - 1)};
    switch (role) {
    case NomRole:
        student->name = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    case SelectRole:
        student->selected=value.toBool();

        emit dataChanged(topLeft, bottomRight, {role});
        return true;
    default:
        return false;
    }
}


void StudentModel::clearSelection() {
    for(size_t i{}; i < m_data.size(); ++i) {
        auto student {std::dynamic_pointer_cast<student_t>(m_data[i])};
        if (student) {
            student->selected = false;
            const auto topLeft {index(static_cast<int>(i), 0)};
            const auto bottomRight {index(static_cast<int>(i), columnCount() - 1)};
            emit dataChanged(topLeft, bottomRight, {SelectRole});
        }
    }
}
