#ifndef PRESENCEMODEL_H
#define PRESENCEMODEL_H
#include "seancemodel.h"
#include <QAbstractListModel>
#include <QObject>

class PresenceModel : public QAbstractListModel {
    Q_OBJECT
public:
    struct Presence {
        int id;
        QString label;
    };

    enum Roles {
        IdRole = Qt::UserRole + 1,
        LabelRole
    };

    explicit PresenceModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int getId(int index) const;
    Q_INVOKABLE QString getLabel(int index) const;
    Q_INVOKABLE int indexOfLabel(const QString& label) const;
    Q_INVOKABLE int getIndexById(int id) const;
public slots:
    void loadFromDatabase();
private:
    QVector<Presence> m_data;


};

#endif // PRESENCEMODEL_H
