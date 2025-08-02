#ifndef MODULEMODEL_H
#define MODULEMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QObject>
#include <QQmlEngine>

struct Module {
    int id;
    QString nom;
    bool selected = false;
};

class ModuleModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        ValeurRole = Qt::UserRole + 1,
        SelectedRole,
        NameRole
    };
    explicit ModuleModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadModulesForSection(const int sectionId,const int semestre);
    Q_INVOKABLE void addModuleForSection(const QString &nom,const int sectionId,const int semestre);
    Q_INVOKABLE void deleteSelected();
    Q_INVOKABLE void updateModule(int index, const QString &nouveauNom);
    Q_INVOKABLE int getId(const int index);
private:
    QList<Module> m_modules;
    QSqlDatabase db;
    void openDatabase();
};

#endif // MODULEMODEL_H
