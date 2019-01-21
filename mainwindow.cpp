#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>

#ifdef win32
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initWindow();
}

MainWindow::~MainWindow()
{
    delete msgBox;
    delete manySendDialog;
    delete ui;
}

void MainWindow::initWindow()
{
    setWindowTitle("串口调试助手BY翟宇");
    ui->funTabWidget->setCurrentIndex(0);
    msgBox = new QMessageBox();
    ui->sendBtn->setEnabled(false);
    ui->manySendBtn->setEnabled(false);

    ui->XYdrawWidget->xAxis->setLabel("x");
    ui->XYdrawWidget->yAxis->setLabel("y");
    ui->XYdrawWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    ui->plotWidget->addGraph();
    //初始化端口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->portCBox->addItem(info.portName());
            ui->receiveTextBrowser->append("检测到端口:");
            ui->receiveTextBrowser->insertPlainText(info.portName());
            serial.close();
        }
    }
    ui->baudCBox->setCurrentIndex(7);
    myComControl.changeFlag = false;
    myComControl.hexSendFlag = false;
    myComControl.hexShowFlag = false;
    myComControl.sendClearFlag = false;

    //初始化发送计数
    sendByteNum = 0;
    receiveByteNum = 0;
    //初始化多项发送窗口
    manySendDialog = new ManySendDialog();
    //发送数据
    connect(manySendDialog,SIGNAL(sendDataSignal(bool,QByteArray)),
            this,SLOT(manySendSlot(bool,QByteArray)));
}

void MainWindow::on_openCloseBtn_clicked()
{
    myComInfo myCom;

    if(ui->portCBox->currentText().isEmpty())
        return;

    if(ui->openCloseBtn->text() == "打开串口")
    {

        myCom.portQString = ui->portCBox->currentText();
        myCom.baudQString = ui->baudCBox->currentText();
        myCom.dataQString = ui->dataCBox->currentText();
        myCom.parityQString = ui->parityCBox->currentText();
        myCom.stopQString = ui->stopCBox->currentText();
        myCom.stateQString = "open";

    }
    else
    {
        ui->openCloseBtn->setText("打开串口");
        ui->readBtn->setEnabled(true);
        ui->portCBox->setEnabled(true);
        ui->baudCBox->setEnabled(true);
        ui->dataCBox->setEnabled(true);
        ui->parityCBox->setEnabled(true);
        ui->stopCBox->setEnabled(true);
        ui->sendBtn->setEnabled(false);
        ui->manySendBtn->setEnabled(false);
        myCom.stateQString = "close";
    }
    emit this->openCloseCom(myCom);
}
//返回打开串口状态
void MainWindow::com_state_slots(bool comState)
{
    if(comState)
    {
        ui->openCloseBtn->setText("关闭串口");
        ui->readBtn->setEnabled(false);
        ui->portCBox->setEnabled(false);
        ui->baudCBox->setEnabled(false);
        ui->dataCBox->setEnabled(false);
        ui->parityCBox->setEnabled(false);
        ui->stopCBox->setEnabled(false);
        ui->sendBtn->setEnabled(true);
        ui->manySendBtn->setEnabled(true);
    }
    else
    {
        msgBox->setInformativeText("打开串口失败");
        msgBox->exec();
    }
}
//接受后台数据显示
void MainWindow::receiveDataSlot(QByteArray data)
{

    QString myStrTemp = QString::fromLocal8Bit(data);

    if(ui->receiveTextBrowser->toPlainText().length()>3500)
        ui->receiveTextBrowser->clear();

    if(myComControl.changeFlag)
        ui->receiveTextBrowser->append(myStrTemp);
    else
        ui->receiveTextBrowser->insertPlainText(myStrTemp);

    ui->receiveTextBrowser->moveCursor(QTextCursor::End);

    if(myComControl.hexShowFlag)
        receiveByteNum += (data.length() - data.length()/3)/2;
    else
        receiveByteNum += data.length();

    ui->receiveGroupBox->setTitle(QString("接收区：已接收%1字节").arg(receiveByteNum));
}
//发送数据
void MainWindow::on_sendBtn_clicked()
{
    if(ui->sendTextEdit->toPlainText().isEmpty())
    {
        return;
    }
    if(myComControl.hexSendFlag)
        sendByteNum += ui->sendTextEdit->toPlainText().length()/2;
    else
        sendByteNum += ui->sendTextEdit->toPlainText().length();
    ui->sendGroupBox->setTitle(QString("发送区：已发送%1字节").arg(sendByteNum));

    ui->sendRecord->append(ui->sendTextEdit->toPlainText());
    emit this->sendDataSignal(myComControl.hexSendFlag,ui->sendTextEdit->toPlainText().toLocal8Bit());
}
//帧换行
void MainWindow::on_changeCheckBox_clicked()
{
    if(ui->changeCheckBox->isChecked())
        myComControl.changeFlag = true;
    else
        myComControl.changeFlag = false;
}
//hex显示
void MainWindow::on_hexShowCheckBox_clicked()
{
    if(ui->hexShowCheckBox->isChecked())
    {
        myComControl.hexShowFlag = true;
    }
    else
    {
        myComControl.hexShowFlag = false;
    }
    emit this->hexShowSignal(myComControl.hexShowFlag);
}
//hex发送
void MainWindow::on_hexSendCheckBox_clicked()
{
    if(ui->hexSendCheckBox->isChecked())
    {
        myComControl.hexSendFlag = true;
    }
    else
    {
        myComControl.hexSendFlag = false;
    }
}
//清空接受区
void MainWindow::on_cleaeReceBtn_clicked()
{
    ui->receiveTextBrowser->clear();
}
//清空发送区
void MainWindow::on_clearSendBtn_clicked()
{
    ui->sendTextEdit->clear();
}
//清空计数
void MainWindow::on_clearContBtn_clicked()
{
    receiveByteNum = 0;
    sendByteNum = 0;
    ui->sendGroupBox->setTitle(QString("发送区：已发送%1字节").arg(sendByteNum));
    ui->receiveGroupBox->setTitle(QString("接收区：已接受%1字节").arg(receiveByteNum));
}

//波形显示
void MainWindow::plotShowSlot(int data)
{
    static long xSite = 0;
    xSite++;
    for(int i=0;i<8;i++)
    {
        ui->plotWidget->graph(i)->addData(xSite, data+i*500);
        ui->plotWidget->graph(i)->removeDataBefore(xSite-300);
    }
    ui->plotWidget->graph(0)->rescaleValueAxis();
    ui->plotWidget->xAxis->setRange(xSite-300, xSite+100);
    ui->plotWidget->replot();
}
//XY绘图仪
void MainWindow::XYdrawSlot(double x, double y)
{
    ui->XYdrawWidget->graph(0)->addData(x, y);
    ui->XYdrawWidget->graph(0)->rescaleValueAxis();
    ui->XYdrawWidget->xAxis->setRange(-1, 1);
    ui->XYdrawWidget->replot();
}
void MainWindow::XYdrawSlot(float x, float y)
{
    ui->XYdrawWidget->graph(0)->addData(x, y);
    ui->XYdrawWidget->graph(0)->rescaleValueAxis();
    ui->XYdrawWidget->xAxis->setRange(-1, 1);
    ui->XYdrawWidget->replot();
}

void MainWindow::on_clearXYdrawBtn_clicked()
{
    ui->XYdrawWidget->graph(0)->removeData(-2,2);
    ui->XYdrawWidget->replot();
}

void MainWindow::on_manySendBtn_clicked()
{
    manySendDialog->setWindowTitle("多项发送设置");
    manySendDialog->show();
}

void MainWindow::manySendSlot(bool hexFlag, QByteArray data)
{
    if(data.isEmpty())
    {
        return;
    }
    if(hexFlag)
        sendByteNum += data.length()/2;
    else
        sendByteNum += data.length();
    ui->sendGroupBox->setTitle(QString("发送区：已发送%1字节").arg(sendByteNum));

    ui->sendRecord->append(QString::fromLocal8Bit(data));
    emit this->sendDataSignal(hexFlag,data);
}
//扫描端口
void MainWindow::on_readBtn_clicked()
{
    ui->portCBox->clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->portCBox->addItem(info.portName());
            ui->receiveTextBrowser->append("检测到端口:");
            ui->receiveTextBrowser->insertPlainText(info.portName());
            serial.close();
        }
    }
}
