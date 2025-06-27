#ifndef ABSENCEDATABASEMANAGER_H
#define ABSENCEDATABASEMANAGER_H

#include <QObject>

class AbsenceDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit AbsenceDatabaseManager(QObject *parent = nullptr);
    Q_INVOKABLE void createDatabase();
    void initialize();
signals:
    void askUserToCreateDatabase();
    void databaseReady();
private:   
    bool databaseExists() const;
    void createSchema();
};

#endif // ABSENCEDATABASEMANAGER_H
