#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QSerialPort>
#include <QDebug>
#include <QThread>
#include <QJsonObject>

#include "database.h"
#include "mytcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool checkSum(QByteArray);
    QJsonObject parseAndSave(QByteArray);
    QDateTime bcdParse(QByteArray);
    QByteArray serialData ;
    void checkData(bool, QByteArray);

private slots:
   void serialButtonClicked(bool checked);
   void tcpButtonClicked(bool checked);
   void serialPortReceived();
   void tcpPortReceived(const Message &message);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;

    void initializeSerialPort();

};
#endif // MAINWINDOW_H
