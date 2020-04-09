
#include "threadobject.h"
#include "mainwindow.h"

ThreadObject::ThreadObject(qintptr sockedID, QObject *parent) : QObject(parent)
{
    this->parent = parent;
    this->socketID = sockedID;
}

void ThreadObject::setSocketID(int socketID)
{
    this->socketID = socketID;
}

void ThreadObject::doTask()
{
    for(int i = 0; i < 100; i++){
        //qDebug() << i;
    }
    socket = new QTcpSocket();
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);

    qDebug() << socketID << " Starting thread";
    if(!socket->setSocketDescriptor(this->socketID)) {
        emit error(socket->error());
        return;
    }
    socket->write("hjgkjgh");
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected!";

        //send
        socket->write("hello server\r\n\r\n\r\n\r\n");

        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

       // socket->close();
    }
    else {
        qDebug() << "Not Connected!";
    }

    qDebug() << socketID << " Client Connected";

}

void ThreadObject::readyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << socketID << " Data in: " << data;
    socket->write(data);
    Message message = Message(data);
    emit dynamic_cast<MyTcpServer*>(parent)->messageSent(message);
}

void ThreadObject::disconnected()
{
    qDebug() << socketID << " Disconnected";
    this->deleteLater();
    exit(0);
}

void ThreadObject::connected()
{
      qDebug() << socketID << " theread connected";
}

