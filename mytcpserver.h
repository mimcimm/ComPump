#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDebug>

#include "message.h"
#include "mythread.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();
    void startServer();

signals:
   void messageSent(const Message &message);

protected:
    void incomingConnection(qintptr socketID) override;



};

#endif // MYTCPSERVER_H
