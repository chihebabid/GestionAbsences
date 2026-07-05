//
// Created by chiheb on 05/07/2026.
//

#include "dbstudent.h"

namespace sqllite {
    void DbStudent::addStudent(const StudentRecord& e) {
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
} // sqllite