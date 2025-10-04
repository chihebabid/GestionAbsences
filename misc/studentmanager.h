#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "basemanager.h"
#include "studentmodel.h"
class StudentManager : public BaseManager
{
    Q_OBJECT
    Q_PROPERTY(StudentModel* mModel READ mModel CONSTANT)
public:
    explicit StudentManager(QObject *parent = nullptr);
    Q_INVOKABLE void fetchForSection(const int id_section);
    Q_INVOKABLE void fetch() override;
    StudentModel* mModel() const {
        return static_cast<StudentModel*>(m_model);
    }
};

#endif // STUDENTMANAGER_H
