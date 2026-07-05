//
// Created by chiheb on 05/07/2026.
//

#ifndef GESTIONABSENCES_DATA_H
#define GESTIONABSENCES_DATA_H
#include <QString>
struct StudentRecord {
    int id;
    QString inscri;
    QString lastName;
    QString firstName;
    QString mail;
    bool selected {false};
};
#endif //GESTIONABSENCES_DATA_H
