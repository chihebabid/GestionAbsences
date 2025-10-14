#ifndef ABSENCEDATABASEMANAGER_H
#define ABSENCEDATABASEMANAGER_H

#include <QObject>
#include <QQmlEngine>

class AbsenceDatabaseManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPair<int,int> m_educationYear READ getEducationYear)
public:
    explicit AbsenceDatabaseManager(QObject *parent = nullptr);
    Q_INVOKABLE void createDatabase();
    void initialize();
    Q_INVOKABLE void openDatabase();
    QPair<int,int> getEducationYear() const;
signals:
    void askUserToCreateDatabase();
    void databaseReady();
private:   
    bool databaseExists() const;
    void createSchema();
    QPair<int,int> m_educationYear;
};

#endif // ABSENCEDATABASEMANAGER_H
