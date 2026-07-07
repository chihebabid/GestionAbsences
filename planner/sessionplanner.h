#ifndef SESSIONPLANNER_H
#define SESSIONPLANNER_H

#include <QObject>
#include <QQmlEngine>
class SessionPlanner : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SessionPlanner(QObject *parent = nullptr);
    Q_INVOKABLE bool addSession(const int moduleId, const int courseType, const QString& date, const QString& startTime, const int duration);
signals:
};

#endif // SESSIONPLANNER_H


