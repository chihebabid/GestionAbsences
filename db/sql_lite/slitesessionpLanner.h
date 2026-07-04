//
// Created by chiheb on 04/07/2026.
//

#ifndef GESTIONABSENCES_SLITESESSIONPLANNER_H
#define GESTIONABSENCES_SLITESESSIONPLANNER_H


class SLiteSessionPLanner : public DBSessionPlanner {
public:
    SLiteSessionPLanner();
    void addSession(const int moduleId, const int courseType, const QString& date, const QString& startTime, const int duration) override;
};


#endif //GESTIONABSENCES_SLITESESSIONPLANNER_H
