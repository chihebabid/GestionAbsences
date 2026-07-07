#ifndef SESSIONMODEL_H
#define SESSIONMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <QList>
#include <QQmlEngine>

struct Session {
    int id;
    QString section;
    QString module;
    QString type;
    QString start;
    QString duration;
    QString date;
};

class SessionModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    enum Roles {
        ValueRole = Qt::UserRole + 1
    };
public:
    explicit SessionModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE int getId(const int index) const;
    Q_INVOKABLE const Session& getSelectedSession() const;
    Q_INVOKABLE void setSelectedIndex(const int index);

public slots:
    void loadSessions();
private:
    QList<Session> m_data;
    int m_current_index {-1};

};

#endif // SESSIONMODEL_H


