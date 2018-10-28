#include "tablenote.h"
#include <QDebug>

TableNote::TableNote(QObject *parent) : QObject(parent){

    getNotesDatabase();

}

void TableNote::createTable(){
    QString str_query;

    str_query = "CREATE TABLE " TABLE_NOTE " ( " TABLE_MONTH " VARCHAR(255) , " TABLE_DAY_N " int , "
            TABLE_DAY_W " VARCHAR(255) , " TABLE_DATE " DATE NOT NULL PRIMARY KEY )";

    QSqlQuery sql_query;

    sql_query.exec(str_query);
}

bool TableNote::setNoteAt(int index, Note note){
    if(index<0 || index>=note_list.size())
        return false;

    note_list[index] = note;

    return true;
}

QVector<Note> TableNote::getNote() const{
    return note_list;
}

bool TableNote::isEmpty() const{
    return m_isEmpty;
}

bool TableNote::addNote(int year, QString month_s, int month_n, QString day_w, int day_n){

    QString date_type = getSqlDate(year,month_n,day_n);

    QString str_query = "SELECT " TABLE_DATE " FROM " TABLE_NOTE " WHERE " TABLE_DATE " = '" + date_type + "'";
    QSqlQuery sql_query;
    sql_query.exec(str_query);

    for(auto &note : note_list){
        if(note.date == date_type)
            return false;
    }

    emit addNoteStart();

    str_query = "INSERT INTO " TABLE_NOTE " ( " TABLE_MONTH " , " TABLE_DAY_N " , " TABLE_DAY_W  " , " TABLE_DATE " ) VALUES ( :month , :day , :day_w , :date )";

    sql_query.prepare(str_query);

    sql_query.bindValue(":month",month_s);
    sql_query.bindValue(":day",day_n);
    sql_query.bindValue(":day_w",day_w);
    sql_query.bindValue(":date","'" + date_type + "'");

    qDebug() << sql_query.exec();

    Note new_note;
    new_note.month = month_s;
    new_note.day = day_n;
    new_note.day_w = day_w;
    new_note.date = date_type;

    note_list.insert(0,new_note);

    setIsEmpty(false);

    emit addNoteEnd();

    return true;

}

void TableNote::deleteNote(QString date, int index){
    emit deleteNoteStart(index);

    QString str_query;

    str_query = "DELETE FROM " TABLE_NOTE " WHERE " TABLE_DATE " = :date";

    QSqlQuery sql_query;

    sql_query.prepare(str_query);

    sql_query.bindValue(":date","'" + date + "'");

    sql_query.exec();

    note_list.remove(index);

    if(!note_list.count())
        setIsEmpty(true);

    emit deleteNoteEnd();

}

void TableNote::setIsEmpty(bool isEmpty){
    if (m_isEmpty == isEmpty)
        return;

    m_isEmpty = isEmpty;
    emit isEmptyChanged(m_isEmpty);
}

QString TableNote::getSqlDate(int year, int month, int day){

    QString sql_format_date = QString::number(year);

    if(month<10)
        sql_format_date += "-0" + QString::number(month);
    else
        sql_format_date += "-" + QString::number(month);

    if(day<10)
        sql_format_date += "-0" + QString::number(day);
    else
        sql_format_date += "-" + QString::number(day);

    return sql_format_date;
}

void TableNote::getNotesDatabase(){
    QString str_query;
    str_query = "SELECT * FROM " TABLE_NOTE;

    QSqlQuery sql_query;
    sql_query.exec(str_query);

    //check database empty
    if(!sql_query.last()){
        setIsEmpty(true);
        return;
    }

    do{

        Note new_note;

        new_note.month = sql_query.value(sql_query.record().indexOf(TABLE_MONTH)).toString();
        new_note.day_w = sql_query.value(sql_query.record().indexOf(TABLE_DAY_W)).toString();
        new_note.day = sql_query.value(sql_query.record().indexOf(TABLE_DAY_N)).toInt();
        new_note.date = sql_query.value(sql_query.record().indexOf(TABLE_DATE)).toString();

        qDebug() << new_note.day;
        qDebug() << new_note.day_w;

        note_list.append(new_note);

    }while((sql_query.previous()));

    setIsEmpty(false);

}
