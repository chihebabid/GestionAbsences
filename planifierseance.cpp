#include "planifierseance.h"

PlanifierSeance::PlanifierSeance(QObject *parent)
    : QObject{parent}
{}


bool PlanifierSeance::ajouterSeance(const int moduleId, const int typeCours, const QString& date, const QString& heureDebut, const int duree) {
    return true;
}
