#include "etudiantsmodel.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QUrl>

EtudiantsModel::EtudiantsModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

int EtudiantsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

int EtudiantsModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return c_columncount;
}

QVariant EtudiantsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};

    const Etudiant &etud = m_data[index.row()];
    switch (role) {
    case InscriRole: return etud.inscri;
    case NomRole: return etud.nom;
    case PrenomRole: return etud.prenom;
    case MailRole: return etud.mail;
    case SelectedRole: return etud.selected;
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return etud.inscri;
        case 1: return etud.nom;
        case 2: return etud.prenom;
        case 3: return etud.mail;
        case 4: return etud.selected;
        }
    }
    return {};
}

QVariant EtudiantsModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
QHash<int, QByteArray> EtudiantsModel::roleNames() const {
    return {
        {InscriRole, "inscri"},{NomRole, "nom"},
        {PrenomRole, "prenom"},
        {MailRole, "mail"},{SelectedRole, "selected"},
    };
}

void EtudiantsModel::loadEtudiantsForSection(const int sectionId) {
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
        Etudiant e;
        e.id = query.value(0).toInt();
        e.inscri = query.value(1).toString();
        e.nom = query.value(2).toString();
        e.prenom = query.value(3).toString();
        e.mail = query.value(4).toString();
        m_data.push_back(e);        
    }
    endResetModel();    
}


bool EtudiantsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    Etudiant &e = m_data[index.row()];
    switch (role) {
    case NomRole:
        e.nom = value.toString();
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

void EtudiantsModel::importCSV(const QUrl &url,const int sectionId) {
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

        Etudiant e;
        e.inscri = champs[0].trimmed();
        e.nom = champs[1].trimmed();
        e.prenom = champs[2].trimmed();
        e.mail = champs[3].trimmed();
        // Exécution requête SQL
        QSqlQuery query;
        query.prepare("INSERT INTO etudiant (inscri,nom,prenom,mail,section_id) VALUES (?,?,?,?,?)");
        query.addBindValue(e.inscri);
        query.addBindValue(e.nom);
        query.addBindValue(e.prenom);
        query.addBindValue(e.mail);
        query.addBindValue(sectionId);
        if (!query.exec()) {
            qWarning() << "Erreur insert from CSV:" << query.lastError();
            return;
        }

    }

    fichier.close();
    loadEtudiantsForSection(sectionId);
}


Qt::ItemFlags EtudiantsModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
