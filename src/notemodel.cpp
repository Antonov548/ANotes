#include "notemodel.h"
#include <QDebug>

NoteModel::NoteModel(QObject *parent)
    : QAbstractListModel(parent){
}

int NoteModel::rowCount(const QModelIndex &parent) const{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_list->getNote().size();
}

QVariant NoteModel::data(const QModelIndex &index, int role) const{

    if (!index.isValid())
        return QVariant();

    const Note note = m_list->getNote().at(index.row());

    switch (role){

    case Month:
        return  QVariant(note.month);
    case Day_w:
        return  QVariant(note.day_w);
    case Day:
        return QVariant(note.day);
    case Count:
        return  QVariant(note.count_c);
    case Date:
        return QVariant(note.date);

    }

    return QVariant();
}

Qt::ItemFlags NoteModel::flags(const QModelIndex &index) const{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> NoteModel::roleNames()const{

    roles[Month] = "month";
    roles[Day_w] = "day_w";
    roles[Day] = "day";
    roles[Count] = "count_c";
    roles[Date] = "date";

    return roles;
}


TableNote *NoteModel::list() const{
    return m_list;
}

QString NoteModel::getProperty(QString role, int index){
    int rol = roles.key(role.toUtf8());

    switch (rol) {
    case Month:
        return m_list->getNote().at(index).month;
    case Day_w:
        return m_list->getNote().at(index).day_w;
    case Day:
        return QString::number(m_list->getNote().at(index).day);
    case Count:
        return QString::number(m_list->getNote().at(index).count_c);
    case Date:
        return m_list->getNote().at(index).date;
    }

    return QString();

}

void NoteModel::resetList(bool isOrder){
    beginResetModel();
    m_list->reorderList(isOrder);
    endResetModel();
}

void NoteModel::setList(TableNote *list){

    beginResetModel();

    m_list = list;

    if (m_list) {
        connect(m_list, &TableNote::addNoteStart, this, [=]() {
            const int index = 0;
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(m_list, &TableNote::addNoteEnd, this, [=]() {
            endInsertRows();
        });

        connect(m_list, &TableNote::deleteNoteStart, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(m_list, &TableNote::deleteNoteEnd, this, [=]() {
            endRemoveRows();
        });
        connect(m_list, &TableNote::moveNoteStart, this, [=](int from, int to) {
            beginMoveRows(QModelIndex(),from,from,QModelIndex(),to);
        });
        connect(m_list, &TableNote::moveNoteEnd, this, [=]() {
            endMoveRows();
        });
        connect(m_list,SIGNAL(resetList(bool)),this,SLOT(resetList(bool)));
        connect(m_list,SIGNAL(updateData(QString,int)),this,SLOT(setProperty(QString,int)));
    }

    emit listChanged(m_list);

    endResetModel();
}

bool NoteModel::setProperty(QString role, int index){
    if(!m_list)
        return  false;

    int number_role = roles.key(role.toUtf8());
    QVector<int> changed_role;
    changed_role << number_role;

    emit dataChanged(NoteModel::index(index), NoteModel::index(index), changed_role);
    return true;
}
