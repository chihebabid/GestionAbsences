#ifndef PLANIFIERSEANCE_H
#define PLANIFIERSEANCE_H

#include <QObject>
#include <QQmlEngine>
class PlanifierSeance : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PlanifierSeance(QObject *parent = nullptr);
    Q_INVOKABLE bool ajouterSeance(const int moduleId, const int typeCours, const QString& date, const QString& heureDebut, const int duree);
signals:
};

#endif // PLANIFIERSEANCE_H
