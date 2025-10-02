#ifndef BASEMANAGER_H
#define BASEMANAGER_H
#include "basemodel.h"
#include <QObject>
class BaseManager : public QObject {
    Q_OBJECT
public:
    explicit BaseManager(BaseModel *model, QObject *parent = nullptr);
    Q_INVOKABLE virtual void fetch()=0;
    Q_INVOKABLE void fakeFetch();
public slots:
    void onUserLoggedIn();
signals:
    void sAdded();
    void sError(const QString &error);
    void sDeleted();
    void sUpdated();
protected:
    BaseModel *m_model;
};

#endif // BASEMANAGER_H
