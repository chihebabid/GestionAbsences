#ifndef SYNTHESETABLEMODEL_H
#define SYNTHESETABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QProperty>
class SyntheseTableModel : public QAbstractTableModel
{
    Q_OBJECT
    enum Roles { InscriRole = Qt::UserRole + 1,NomRole, PrenomRole,NbAbsenceRole,PourcentageRole };
    static constexpr int c_columncount {5};
    struct EtudiantAbsence {
        int id;
        QString inscri;
        QString nom;
        QString prenom;
        int nbAbsences;
    };
    Q_PROPERTY(int nbSeances READ nbSeances WRITE setNbSeances NOTIFY nbSeancesChanged)
public:
    explicit SyntheseTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Q_INVOKABLE void loadAbsencesListForModule(const int idModule);
    void setNbSeances(const int);
    int nbSeances() const;
signals:
    void nbSeancesChanged();
private:
    QList<EtudiantAbsence> m_data;
    Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(SyntheseTableModel,int,m_nb_seances,0,&SyntheseTableModel::nbSeancesChanged)

};

#endif // SYNTHESETABLEMODEL_H
