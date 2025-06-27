#ifndef SECTIONMODEL_H
#define SECTIONMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QList>

struct Section {
    int id;
    QString nom;
    bool selected = false;
};

class SectionModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        ValeurRole = Qt::UserRole + 1,
        SelectedRole,
        NameRole
    };

    explicit SectionModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;


    Q_INVOKABLE void insertSection(const QString &nom);
    Q_INVOKABLE void updateSection(int index, const QString &nouveauNom);
    Q_INVOKABLE void deleteSelected();
    Q_INVOKABLE void clearSelection();
    Q_INVOKABLE int getSectionId(int indexModel) const;
public slots:
    void loadSections();
private:
    QList<Section> m_sections;
    void openDatabase();
};

#endif // SECTIONMODEL_H
