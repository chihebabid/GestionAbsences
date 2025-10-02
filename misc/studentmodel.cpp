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
        case Qt::DisplayRole:
        switch (index.column()) {
            case 0: return student->inscri;
            case 1: return student->name;
            case 2: return student->firstName;
            case 3: return student->mail;
        }
    }
    return {};
}

QHash<int, QByteArray> StudentModel::roleNames() const {
    return {
        {InscriRole, "inscri"},{NomRole, "nom"},
        {PrenomRole, "prenom"},
        {MailRole, "mail"}
    };
}
