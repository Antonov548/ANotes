#pragma once

#include <QAbstractListModel>
#include "tablenote.h"

class NoteModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(TableNote* list READ list WRITE setList NOTIFY listChanged)

public:
    explicit NoteModel(QObject *parent = nullptr);

    enum{
        Month = Qt::UserRole,
        Day_w,
        Day,
        Count,
        Date
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    TableNote* list() const;

public slots:
    void setList(TableNote* list);
    QString getProperty(QString role,int index);
    bool setProperty(QString,int);
    void resetList(bool);

signals:
    void listChanged(TableNote* list);

private:
    TableNote* m_list;
    mutable QHash<int,QByteArray> roles;
    int m_noteCount;
};

