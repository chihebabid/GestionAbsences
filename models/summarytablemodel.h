#ifndef SUMMARYTABLEMODEL_H
#define SUMMARYTABLEMODEL_H
#include "sessionmodel.h"
#include "misc.h"
#include <QAbstractTableModel>
#include <QObject>
#include <QProperty>

struct StudentAbsence {
    int id;
    QString inscri;
    QString lastName;
    QString firstName;
    int nbAbsences;
};

class SummaryTableModel : public QAbstractTableModel
{
    Q_OBJECT
    enum Roles { InscriRole = Qt::UserRole + 1,LastNameRole, FirstNameRole,NbAbsenceRole,PercentageRole };
    static constexpr int c_columncount {5};

    Q_PROPERTY(int nbSessions READ nbSessions WRITE setNbSessions NOTIFY nbSessionsChanged)
    Q_PROPERTY(int nbTotalSessions READ nbTotalSessions WRITE setNbTotalSessions NOTIFY nbSessionsChanged)
public:
    explicit SummaryTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void loadAbsencesListForModule(const int idModule,const QVariantList &typeIds);
    void setNbSessions(const int);
    int nbSessions() const;
    void setNbTotalSessions(const int);
    int nbTotalSessions() const;
    am::Info_t getCurrentInfo() const;
    const QList<StudentAbsence> &getStudentList() const;
    const QString getCourseTypeList() const;
signals:
    void nbSessionsChanged();
private:
    QList<StudentAbsence> m_data;
    am::Info_t m_info;
    QString m_types_course;
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(SummaryTableModel,int,m_nb_sessions,0,&SummaryTableModel::nbSessionsChanged)
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(SummaryTableModel,int,m_nb_total_sessions,0,&SummaryTableModel::nbSessionsChanged)

};

#endif // SUMMARYTABLEMODEL_H


