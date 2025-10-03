#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "basemanager.h"

class StudentManager : public BaseManager
{
    Q_OBJECT
public:
    explicit StudentManager(QObject *parent = nullptr);
    Q_INVOKABLE void fetchForSection(const int id_section);
};

#endif // STUDENTMANAGER_H
