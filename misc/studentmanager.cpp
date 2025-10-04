#include "studentmanager.h"
#include "studentmodel.h"
#include <QSqlError>
#include <QSqlQuery>

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
