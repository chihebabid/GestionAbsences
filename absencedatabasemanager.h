#ifndef ABSENCEDATABASEMANAGER_H
#define ABSENCEDATABASEMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QProperty>

class AbsenceDatabaseManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList educationYear READ getEducationYear NOTIFY educationYearChanged)
public:
    explicit AbsenceDatabaseManager(QObject *parent = nullptr);
    Q_INVOKABLE void createDatabase();
    void initialize();
    Q_INVOKABLE void openDatabase();
    QVariantList getEducationYear() const;
signals:
    void askUserToCreateDatabase();
    void databaseReady();
    void educationYearChanged();
private:   
    bool databaseExists() const;
    void createSchema();
    void init();
    QPair<int,int> m_educationYear;
    Q_OBJECT_BINDABLE_PROPERTY(AbsenceDatabaseManager,QVariantList, educationYear, &AbsenceDatabaseManager::educationYearChanged)
};

#endif // ABSENCEDATABASEMANAGER_H
