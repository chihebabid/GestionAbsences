#include "studentmanager.h"
#include "studentmodel.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QUrl>

StudentManager::StudentManager(QObject *parent)
    : BaseManager{new StudentModel{parent},parent}
{}


void StudentManager::fetchForSection(const int id_section) {
    if (!QSqlDatabase::database().isOpen()) {
        qWarning() << "Base de données non ouverte !";
        emit sError(QString{"Base de données non ouverte !"});
        return;
    }
    //beginResetModel();
    //m_data.clear();
    std::vector<std::shared_ptr<base_struct_t>> students;
    QSqlQuery query;
    query.prepare(R"(
        SELECT id,inscri,nom, prenom,mail
        FROM etudiant
        WHERE section_id = ? )");

    query.addBindValue(id_section);

    if (!query.exec()) {
        qWarning() << "Erreur requête modules:" << query.lastError();
        emit sError(query.lastError().text());
        return;
    }

    while (query.next()) {
        student_t e;
        e.id = query.value(0).toInt();
        e.inscri = query.value(1).toString();
        e.name = query.value(2).toString();
        e.firstName = query.value(3).toString();
        e.mail = query.value(4).toString();
        e.sectionId=id_section;
        students.emplace_back(std::make_shared<student_t>(e));
    }
    m_model->set(students);
}

void StudentManager::fetch() {

}

void StudentManager::importCSV(const QUrl &url,const int sectionId) {
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

        student_t e;
        e.inscri = champs[0].trimmed();
        e.name = champs[1].trimmed();
        e.firstName = champs[2].trimmed();
        e.mail = champs[3].trimmed();
        // Exécution requête SQL
        QSqlQuery query;
        query.prepare("INSERT INTO etudiant (inscri,nom,prenom,mail,section_id) VALUES (?,?,?,?,?)");
        query.addBindValue(e.inscri);
        query.addBindValue(e.name);
        query.addBindValue(e.firstName);
        query.addBindValue(e.mail);
        query.addBindValue(sectionId);
        if (!query.exec()) {
            qWarning() << "Erreur insert from CSV:" << query.lastError();
            return;
        }

    }

    fichier.close();
    fetchForSection(sectionId);
}
