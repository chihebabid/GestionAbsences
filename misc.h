#ifndef MISC_H
#define MISC_H
#include <QString>
#include <QSqlDatabase>
namespace am {

    struct DBInfo {
        QSqlDatabase db;
        QString dbName;
    };
    extern struct DBInfo dbInfo;

    struct Etudiant {
        int id;
        QString inscri;
        QString nom;
        QString prenom;
        int presence;
    };
}
#endif // MISC_H
