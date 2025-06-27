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
}
#endif // MISC_H
