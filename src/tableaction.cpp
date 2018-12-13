#include "tableaction.h"
#include <QDebug>

TableAction::TableAction(QObject *parent) : QObject(parent),m_isEmpty(true){

}

void TableAction::createTable(){
    QString str_query;
    QSqlQuery sql_query;

    str_query = "CREATE TABLE " TABLE_ACTION " ( " TABLE_INFO " text, " TABLE_DONE " int , " TABLE_INDEX " int , " TABLE_DATE " text NOT NULL)";
    sql_query.exec(str_query);
}

QList<Action> TableAction::getAction() const{
    return action_list;
}

bool TableAction::isEmpty() const{
    return m_isEmpty;
}

void TableAction::addAction(QString text){

    emit addNoteStart();

    Action new_action;
    new_action.information = text;
    new_action.isDone = false;
    new_action.date = "";

    action_list.insert(0,new_action);
    m_isEmpty = action_list.isEmpty();
    emit addNoteEnd();
}

void TableAction::resetList(){
    action_list.clear();
    m_isEmpty = true;
}

void TableAction::deleteAction(int index){
    emit deleteNoteStart(index);
    action_list.removeAt(index);
    m_isEmpty = action_list.isEmpty();
    emit deleteNoteEnd();
}

void TableAction::addActionsDatabase(QString date){

    int index = action_list.count()-1;

    for(auto &action : action_list){

        QString str_query;
        QSqlQuery sql_query;

        str_query = "INSERT INTO " TABLE_ACTION " ( " TABLE_INFO " , " TABLE_DONE " , " TABLE_INDEX " , " TABLE_DATE  "  ) VALUES ( :info, :done, :index, :date )";

        sql_query.prepare(str_query);

        sql_query.bindValue(":info",action.information);
        sql_query.bindValue(":done",action.isDone);
        sql_query.bindValue(":index",index);
        sql_query.bindValue(":date",date);

        sql_query.exec();
        index--;
    }
    action_list.clear();
}

void TableAction::getActionsDatabase(QString date){
    resetList();

    QString str_query;
    QSqlQuery sql_query;

    str_query = "SELECT * FROM " TABLE_ACTION " WHERE " TABLE_DATE "='" + date + "'";
    sql_query.exec(str_query);

    if(!sql_query.last()){
        setIsEmpty(true);
        return;
    }

    do{
        Action new_action;

        new_action.date = sql_query.value(sql_query.record().indexOf(TABLE_DATE)).toString();
        new_action.information = sql_query.value(sql_query.record().indexOf(TABLE_INFO)).toString();
        new_action.isDone = sql_query.value(sql_query.record().indexOf(TABLE_DONE)).toBool();

        action_list.append(new_action);

    }while((sql_query.previous()));

    setIsEmpty(false);
}

void TableAction::deleteActionsDatabase(QString date){
    QString str_query;
    QSqlQuery sql_query;

    str_query = "DELETE FROM " TABLE_ACTION " WHERE " TABLE_DATE "='" + date + "'";
    sql_query.exec(str_query);
}

void TableAction::setDone(QString date, int index, bool done){
    QString str_query;
    QSqlQuery sql_query;

    str_query = "UPDATE " TABLE_ACTION " SET " TABLE_DONE " = :done WHERE " TABLE_DATE "=:date AND " TABLE_INDEX "=:index";
    sql_query.prepare(str_query);

    sql_query.bindValue(":done",int(done));
    sql_query.bindValue(":date",date);
    sql_query.bindValue(":index",index);

    sql_query.exec();
    action_list[index].isDone = done;

    emit updateData("done",index);
}

void TableAction::setIsEmpty(bool isEmpty){
    if (m_isEmpty == isEmpty)
        return;

    m_isEmpty = isEmpty;
    emit isEmptyChanged(m_isEmpty);
}