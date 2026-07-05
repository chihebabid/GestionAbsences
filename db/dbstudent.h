#ifndef DBSTUDENT_H
#define DBSTUDENT_H
#include "data.h"
class DbStudent {
public:
    virtual void addStudent(const StudentRecord& ) = 0;
};

#endif // DBSTUDENT_H
