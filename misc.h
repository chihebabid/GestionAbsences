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

    QString calculateEndTime(const QString &start, const int minutes);

    struct Info_t {
        QString section;
        QString module;
        int semestre;
    };

}
#endif // MISC_H
