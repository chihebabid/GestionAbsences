#ifndef TYPECOURSMODEL_H
#define TYPECOURSMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

struct TypeCours {
    int id;
    QString nom;
    bool hasSeances = false;
};

class TypeCoursModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TypeCoursModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int getId(int index) const;
    Q_INVOKABLE QString getNom(int index) const;

    Q_INVOKABLE void load();
    Q_INVOKABLE void loadTypesCoursForModule(const int idModule);
private:
    QList<TypeCours> m_data;
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NomRole,
        HasSeancesRole
    };
};

#endif // TYPECOURSMODEL_H
