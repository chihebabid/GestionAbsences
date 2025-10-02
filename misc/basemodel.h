#ifndef BASEMODEL_H
#define BASEMODEL_H
#include <QObject>
#include <QQmlEngine>
#include <QAbstractTableModel>
#include "globals.h"

class BaseModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit BaseModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void reset();
    void set(const std::vector<std::shared_ptr<base_struct_t>> &data);
protected:
    std::vector<std::shared_ptr<base_struct_t>> m_data;
    int _columnCount;
};

#endif // BASEMODEL_H
