#ifndef ABSENCEMODEL_H
#define ABSENCEMODEL_H
#include <QAbstractTableModel>
#include <QObject>

class AbsenceModel : QAbstractTableModel {
    Q_OBJECT
    struct Etudiant {
        int id;
        QString inscri;
        QString nom;
        QString prenom;
        int presence;
    };
    enum Roles { InscriRole = Qt::UserRole + 1,NomRole, PrenomRole,PresenceRole };
    static constexpr int c_columncount {4};
public:
    explicit AbsenceModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
private:
    QList<Etudiant> m_data;
};

#endif // ABSENCEMODEL_H
