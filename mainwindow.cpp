#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QErrorMessage>
#include <QDateTime>

#include <string>
#include <sstream>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->serialServerButton,SIGNAL(clicked(bool)),this,SLOT(serialButtonClicked(bool)));
    connect(ui->tcpServerButton,SIGNAL(clicked(bool)),this,SLOT(tcpButtonClicked(bool)));
}

MainWindow::~MainWindow()
{
    serial->close();
    delete ui;
}

void MainWindow::initializeSerialPort()
{
    QThread *serialThread = new QThread;
    serial = new QSerialPort(this);
    serial->setParent(NULL);
    serial->moveToThread(serialThread);
    serial->setPortName("COM15");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serialThread,SIGNAL(finished()),serialThread,SLOT(deleteLater()));
    connect(serialThread,SIGNAL(finished()),serial,SLOT(deleteLater()));
    connect(serialThread,&QThread::started,[=]()->void{
        if(!serial->open(QIODevice::ReadWrite)){
            qDebug() << "cihaz açılmıyor";
            qDebug() << "error code = " << serial->error();
            qDebug() << "error string = " << serial->errorString();
            serialThread->quit();
        }else{
            qDebug() << "cihaz açıldı";
        }
    });
    serialThread->start();
    serial->write("written");
}

void MainWindow::serialPortReceived()
{
    QByteArray bytArrayData = serial->readAll();
    //QString stringData = QString::fromStdString(bytArrayData.toStdString());
    qDebug() << "bytArrayData  Result:  " << bytArrayData;
    qDebug() << "bytArrayData  Lenght:  " << bytArrayData.length();
    serialData.append(bytArrayData);
    qDebug() << "serialData  :  " << serialData;
    checkData(true, serialData);

    serial->flush();
}

void MainWindow::tcpPortReceived(const Message &message)
{
    qDebug() << "TCP Message :  " << message.body();
    checkData(false, message.body());
}

void MainWindow::checkData(bool isSerial,QByteArray checkData)
{
    if(checkData.length()>1){
        if(checkData.at(0) != static_cast<char> (0xAF) || checkData.at(1) != static_cast<char> (0xAF) ){
            qDebug() << "Wrong Data Rejected" ;
            if(isSerial) serialData = "";
            return;
        }

        if(checkData.data()[checkData.length()-1] == static_cast<char> (0xFA)
                && checkData.data()[checkData.length()-2] == static_cast<char>(0xFA)) {
               if(checkSum(checkData)){
                    QJsonObject obj = parseAndSave(checkData);
                    qDebug() << "PAKET DOĞRU " ;
                    if(isSerial) ui->serialEdit->setText(obj.value("pumpNo").toString()
                                                         +"\n" + obj.value("orderDate").toString()
                                                         +"\n" + obj.value("currentDate").toString());
                    else ui->tcpEdit->setText(obj.value("pumpNo").toString()
                                               +"\n" + obj.value("orderDate").toString()
                                               +"\n" + obj.value("currentDate").toString());
                }else{
                    qDebug() << "PAKET YANLIŞ " ;
                }
                if(isSerial) serialData = "";
        }
    }
}

bool MainWindow::checkSum(QByteArray XORArray)
{
    qDebug() << "XOR size" <<XORArray.size();
    QByteArray arr;
    char key = XORArray[2];
    qDebug() << "key11 ::  " <<key;
    for (int i = 3; i < XORArray.size()-3; i++)
        key = key ^ XORArray[i];

    qDebug() << "key ::  " <<key;
    if(key == XORArray[XORArray.size()-3])
        return true ;
     else
        return false ;
}

QJsonObject MainWindow::parseAndSave(QByteArray data)
{
    int order  = data[2];
    int pumpNo = data[3];
    qDebug() << "pumpNo ::  " << pumpNo;
    qDebug() << "data[4] ::  " << data[4];

    QDateTime orderDate = bcdParse(data);
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    QDateTime currentDate;
    currentDate.setTime_t(time/1000);
    qDebug() << currentDate.toString("yyyy-MM-dd hh:mm:ss");

    Database db;
    if(order == 1)
        db.addData(pumpNo, orderDate, currentDate);

    db.deleteLater();
    QJsonObject obj;
    obj["pumpNo"] = QString::number(pumpNo);
    obj["orderDate"] = orderDate.toString();
    obj["currentDate"] = currentDate.toString();

    return obj;
}

QDateTime MainWindow::bcdParse(QByteArray data)
{
    QByteArray yearArray,monthArray,dayArray,hourArray,minuteArray,secondArray;
    QString yearStr,monthStr,dayStr,hourStr,minuteStr,secondStr;

    yearArray.append(data[4]);  yearArray.append(data[5]);
    monthArray.append(data[6]);
    dayArray.append(data[7]);
    hourArray.append(data[8]);
    minuteArray.append(data[9]);
    secondArray.append(data[10]);

    yearStr = yearArray.toHex();
    monthStr = monthArray.toHex();
    dayStr = dayArray.toHex();
    hourStr = hourArray.toHex();
    minuteStr = minuteArray.toHex();
    secondStr = secondArray.toHex();

    QDateTime date(QDate(yearStr.toInt(), monthStr.toInt(), dayStr.toInt()),
                        QTime(hourStr.toInt(), minuteStr.toInt(), secondStr.toInt()));

    qDebug() <<"datem Kendi Parsem" << date.toString("yyyy-MM-dd hh:mm:ss");
    return  date;
}

void MainWindow::serialButtonClicked(bool checked)
{
    if(checked == false){
        initializeSerialPort();
        connect(serial,SIGNAL(readyRead()),this,SLOT(serialPortReceived()));
        ui->serialServerButton->setEnabled(false);
    }

}

void MainWindow::tcpButtonClicked(bool checked)
{
    if(checked == false){
        MyTcpServer *tcpServer = new MyTcpServer();
        tcpServer->startServer();
        typedef Message msg;
        qRegisterMetaType<msg>("Message&");
        connect(tcpServer,SIGNAL(messageSent(Message&)),this,SLOT(tcpPortReceived(Message&)));
    }
    ui->tcpServerButton->setEnabled(false);
}


