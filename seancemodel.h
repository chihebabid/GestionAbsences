#ifndef SEANCEMODEL_H
#define SEANCEMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <QList>

struct Seance {
    int id;
    QString section;
    QString module;
    QString type;
    QString debut;
    QString duree;
    QString date;
};

class SeanceModel : public QAbstractListModel {
    Q_OBJECT
    enum Roles {
        ValeurRole = Qt::UserRole + 1
    };
public:
    explicit SeanceModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int getId(const int index) const;
    Q_INVOKABLE Seance getSeance(const int index);
public slots:
    void loadSeances();
private:
    QList<Seance> m_data;
};

#endif // SEANCEMODEL_H
