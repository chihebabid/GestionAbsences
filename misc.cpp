#include "misc.h"
#include <QTime>
namespace am {
    struct DBInfo dbInfo; // {.dbName="absences.db"};

    QString calculateEndTime(const QString &start, const int minutes) {
        QTime debut {QTime::fromString(start, "hh:mm")};
        QTime fin {debut.addSecs(minutes * 60)};
        return fin.toString("hh:mm");
    }
}
