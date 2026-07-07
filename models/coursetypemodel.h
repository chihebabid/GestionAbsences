#ifndef COURSETYPEMODEL_H
#define COURSETYPEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

struct CourseType {
    int id;
    QString name;
    bool hasSessions = false;
};

class CourseTypeModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit CourseTypeModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int getId(int index) const;
    Q_INVOKABLE QString getName(int index) const;

    Q_INVOKABLE void load();
    Q_INVOKABLE void loadCourseTypesForModule(const int idModule);
private:
    QList<CourseType> m_data;
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        HasSessionsRole
    };
};

#endif // COURSETYPEMODEL_H


