#ifndef PLANIFIERSEANCE_H
#define PLANIFIERSEANCE_H

#include <QObject>

class PlanifierSeance : public QObject
{
    Q_OBJECT
public:
    explicit PlanifierSeance(QObject *parent = nullptr);
    Q_INVOKABLE bool ajouterSeance(const int moduleId, const int typeCours, const QString& date, const QString& heureDebut, const int duree);
signals:
};

#endif // PLANIFIERSEANCE_H
