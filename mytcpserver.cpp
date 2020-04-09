#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::startServer()
{
    if(!this->listen(QHostAddress::Any,1234))
        qDebug() << "Could not start server";
    else
        qDebug() << "Listening...";
}

void MyTcpServer::incomingConnection(qintptr socketID)
{
    qDebug() << socketID << " Connecting...";
    MyThread *thread = new MyThread(socketID,this);
    connect(thread, SIGNAL(finished()),thread, SLOT(deleteLater()));
    thread->start();
}
