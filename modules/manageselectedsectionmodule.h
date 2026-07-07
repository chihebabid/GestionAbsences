#ifndef MANAGESELECTEDSECTIONMODULE_H
#define MANAGESELECTEDSECTIONMODULE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>
#include <QStringListModel>
#include <QQmlEngine>

class ManageSelectedSectionModule : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QStringList sectionsModel READ sectionsModel NOTIFY sectionsModelChanged)
public:
    explicit ManageSelectedSectionModule(QObject *parent = nullptr);
     QStringList sectionsModel() const { return m_sectionsModel->stringList(); }
public slots:
    void loadSectionsFromDatabase();
    Q_INVOKABLE bool insertSection(QString v);
signals:
    void sectionsModelChanged();
private:
     QStringListModel *m_sectionsModel;
};

#endif // MANAGESELECTEDSECTIONMODULE_H


