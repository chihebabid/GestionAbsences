#ifndef DBPLANNER_H
#define DBPLANNER_H
#include "misc.h"
class DBSessionPlanner {
public:
    DBSessionPlanner();
    virtual void addSession(const int moduleId, const int courseType, const QString& date, const QString& startTime, const int duration)
    = 0;
};

#endif // DBPLANNER_H
