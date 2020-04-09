#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{


}

void Database::addData(int pumpNo, QDateTime orderTime ,QDateTime currentTime)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/a.kurtvuran/Documents/SQLITEComPump.db");
    if(!db.open())
         qDebug() << "problem opening";

    QString data = "INSERT INTO test (pumpNo, orderDate, currentDate) VALUES(?,?,?);" ;
    QSqlQuery query;
    query.prepare(data);
    //query.addBindValue(id);
    query.addBindValue(pumpNo);
    query.addBindValue(orderTime);
    query.addBindValue(currentTime);

    if(!query.exec()){
         qDebug() << query.lastError();
         qDebug() << "Problem adding value";
    }
    db.close();
}

