//
// Created by chiheb on 05/07/2026.
//

#ifndef GESTIONABSENCES_DBSTUDENT_H
#define GESTIONABSENCES_DBSTUDENT_H
#include "db/dbstudent.h"

namespace sqllite {
    class DbStudent : public ::DbStudent{
        virtual void addStudent(const StudentRecord&) override;
    };
} // sqllite

#endif //GESTIONABSENCES_DBSTUDENT_H
