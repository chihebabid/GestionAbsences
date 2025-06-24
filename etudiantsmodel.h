#ifndef ETUDIANTSMODEL_H
#define ETUDIANTSMODEL_H

#include <QAbstractTableModel>

#include <QObject>
struct Etudiant {
    int id;
    QString inscri;
    QString nom;
    QString prenom;
    QString mail;
};
class EtudiantsModel : public QAbstractTableModel
{
    Q_OBJECT
    enum Roles { InscriRole = Qt::UserRole + 1,NomRole, PrenomRole,MailRole };
    static constexpr int c_columncount {4};
public:
    explicit EtudiantsModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE void loadEtudiantsForSection(const int sectionId);
    Q_INVOKABLE void importCSV(const QString &,const int sectionId);
private:
    QList<Etudiant> m_data;
};

#endif // ETUDIANTSMODEL_H
