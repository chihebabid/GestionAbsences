#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H
#include "globals.h"
#include "basemodel.h"

struct student_t : public base_struct_t {
    int id;
    QString inscri;
    QString name;
    QString firstName;
    QString mail;
    int sectionId;
};

class StudentModel : public BaseModel {
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles { InscriRole = Qt::UserRole + 1,NomRole, PrenomRole,MailRole};
    explicit StudentModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // STUDENTMODEL_H
