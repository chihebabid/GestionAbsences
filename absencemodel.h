#ifndef ABSENCEMODEL_H
#define ABSENCEMODEL_H
#include <QAbstractTableModel>
#include <QObject>

class AbsenceModel : public QAbstractTableModel {
    Q_OBJECT

    enum Roles { InscriRole = Qt::UserRole + 1,NomRole, PrenomRole,PresenceRole };
    static constexpr int c_columncount {4};
public:
    struct Etudiant {
        int id;
        QString inscri;
        QString nom;
        QString prenom;
        int presence;
    };
    explicit AbsenceModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE bool setPresence(int row, int presenceId);
    Q_INVOKABLE void loadEtudiantsForSeance(const int seanceId);
private:
    QList<Etudiant> m_data;
};

#endif // ABSENCEMODEL_H
