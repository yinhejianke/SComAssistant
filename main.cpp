#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include "datamanage.h"
#include "datalogthrad.h"
#include "stdio.h"

#ifdef win32
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    DataManage *dataMana = new DataManage();
    DataLogThread *dataLogThread = new DataLogThread();

    //写数据日志
    QObject::connect(dataMana,SIGNAL(saveDataSignal(QByteArray, bool, QString)),
                     dataLogThread,SLOT(saveDataLog(QByteArray, bool, QString)),Qt::QueuedConnection);
    //开关串口
    QObject::connect(&w,SIGNAL(openCloseCom(myComInfo)),
                     dataMana,SLOT(openCloseSlot(myComInfo)),Qt::QueuedConnection);
    //返回串口状态
    QObject::connect(dataMana,SIGNAL(comStateSignal(bool)),
                     &w,SLOT(com_state_slots(bool)),Qt::QueuedConnection);
    //接受数据
    QObject::connect(dataMana,SIGNAL(receiveDataSignal(QByteArray)),
                     &w,SLOT(receiveDataSlot(QByteArray)),Qt::QueuedConnection);
    //发送数据
    QObject::connect(&w,SIGNAL(sendDataSignal(bool,QByteArray)),
                     dataMana,SLOT(sendDataSlot(bool,QByteArray)),Qt::QueuedConnection);
    //hex显示
    QObject::connect(&w,SIGNAL(hexShowSignal(bool)),
                     dataMana,SLOT(hexShowSlot(bool)),Qt::QueuedConnection);
    //波形显示
    QObject::connect(dataMana,SIGNAL(plotShowSignal(int)),
                     &w,SLOT(plotShowSlot(int)),Qt::QueuedConnection);
    //XY绘图仪
    QObject::connect(dataMana,SIGNAL(XYdrawSignal(double,double)),
                     &w,SLOT(XYdrawSlot(double,double)),Qt::QueuedConnection);
    QObject::connect(dataMana,SIGNAL(XYdrawSignal(float,float)),
                     &w,SLOT(XYdrawSlot(float,float)),Qt::QueuedConnection);

    dataMana->start();
    dataLogThread->start();

    return a.exec();
}
