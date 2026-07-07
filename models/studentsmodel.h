#ifndef STUDENTSMODEL_H
#define STUDENTSMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QQmlEngine>

struct StudentRecord {
    int id;
    QString inscri;
    QString lastName;
    QString firstName;
    QString mail;
    bool selected {false};
};

class StudentsModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    enum Roles { InscriRole = Qt::UserRole + 1,NomRole, PrenomRole,MailRole,SelectedRole };
    static constexpr int c_columncount {4};
public:
    explicit StudentsModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void loadStudentsForSection(const int sectionId);
    Q_INVOKABLE void importCSV(const QUrl &,const int sectionId);
private:
    QList<StudentRecord> m_data;
};

#endif // STUDENTSMODEL_H


