#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QErrorMessage>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    void addData(int pumpNo, QDateTime orderTime ,QDateTime currentTime);
private:
    QSqlDatabase db ;
signals:

};

#endif // DATABASE_H
