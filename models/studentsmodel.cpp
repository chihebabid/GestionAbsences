#include "studentsmodel.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QUrl>

StudentsModel::StudentsModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int StudentsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int StudentsModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return c_columncount;
}

QVariant StudentsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};

    const StudentRecord &stud = m_data[index.row()];
    switch (role) {
    case InscriRole: return stud.inscri;
    case NomRole: return stud.lastName;
    case PrenomRole: return stud.firstName;
    case MailRole: return stud.mail;
    case SelectedRole: return stud.selected;
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return stud.inscri;
        case 1: return stud.firstName;
        case 2: return stud.lastName;
        case 3: return stud.mail;
        case 4: return stud.selected;
        }
    }
    return {};
}

QVariant StudentsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Inscription";
        case 1: return "Nom";
        case 2: return "Prénom";
        case 3: return "Mail";
        }
    }
    return {};
}
QHash<int, QByteArray> StudentsModel::roleNames() const {
    return {
        {InscriRole, "inscri"},{NomRole, "nom"},
        {PrenomRole, "prenom"},
        {MailRole, "mail"},{SelectedRole, "selected"},
    };
}

void StudentsModel::loadStudentsForSection(const int sectionId) {
    if (!QSqlDatabase::database().isOpen()) {
        qWarning() << "Base de données non ouverte !";
    }
    beginResetModel();
    m_data.clear();
    QSqlQuery query;
    query.prepare(R"(
        SELECT id,inscri,nom, prenom,mail
        FROM etudiant
        WHERE section_id = ? )");

    query.addBindValue(sectionId);

    if (!query.exec()) {
        qWarning() << "Erreur requête modules:" << query.lastError();
        endResetModel();
        return;
    }

    while (query.next()) {
        StudentRecord e;
        e.id = query.value(0).toInt();
        e.inscri = query.value(1).toString();
        e.lastName = query.value(2).toString();
        e.firstName = query.value(3).toString();
        e.mail = query.value(4).toString();
        m_data.push_back(e);
    }
    endResetModel();
}


bool StudentsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    StudentRecord &e = m_data[index.row()];
    switch (role) {
    case NomRole:
        e.lastName = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    case SelectedRole:
        e.selected=value.toBool();
        emit dataChanged(index, index, {role});
        return true;
    default:
        return false;
    }
}

void StudentsModel::importCSV(const QUrl &url,const int sectionId) {
    QString cheminFichier {QUrl(url).toLocalFile()};
    QFile fichier(cheminFichier);

    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossible d’ouvrir le fichier :" << cheminFichier;
        return ;
    }
    QTextStream flux(&fichier);
    bool premiereLigne = true;

    while (!flux.atEnd()) {
        QString ligne = flux.readLine().trimmed();
        if (ligne.isEmpty())
            continue;

        if (premiereLigne) {
            premiereLigne = false; // ignorer l'entête
            continue;
        }

        QStringList champs = ligne.split(',', Qt::SkipEmptyParts);
        qDebug()<<"Nombre de champs:"<<champs.size();
        if (champs.size() < 4)
            continue;

        StudentRecord e;
        e.inscri = champs[0].trimmed();
        e.lastName = champs[1].trimmed();
        e.firstName = champs[2].trimmed();
        e.mail = champs[3].trimmed();
        // Exécution requête SQL
        QSqlQuery query;
        query.prepare("INSERT INTO etudiant (inscri,nom,prenom,mail,section_id) VALUES (?,?,?,?,?)");
        query.addBindValue(e.inscri);
        query.addBindValue(e.lastName);
        query.addBindValue(e.firstName);
        query.addBindValue(e.mail);
        query.addBindValue(sectionId);
        if (!query.exec()) {
            qWarning() << "Erreur insert from CSV:" << query.lastError();
            return;
        }

    }

    fichier.close();
    loadStudentsForSection(sectionId);
}


Qt::ItemFlags StudentsModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}


