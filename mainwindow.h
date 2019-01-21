#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDialog>
#include <QRect>
#include <QMessageBox>
#include "datamanage.h"
#include "qcustomplot.h"
#include "manysenddialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    

    myComCon myComControl;

signals:
    void openCloseCom(myComInfo);
    void sendDataSignal(bool,QByteArray);
    void hexShowSignal(bool);

public slots:

    void plotShowSlot(int data);
    void XYdrawSlot(double x,double y);
    void XYdrawSlot(float x,float y);



private slots:
    void receiveDataSlot(QByteArray data);

    void on_sendBtn_clicked();

    void on_openCloseBtn_clicked();

    void on_changeCheckBox_clicked();

    void on_hexShowCheckBox_clicked();

    void on_hexSendCheckBox_clicked();

    void on_cleaeReceBtn_clicked();

    void on_clearSendBtn_clicked();

    void on_clearContBtn_clicked();

    void on_clearXYdrawBtn_clicked();

    void on_manySendBtn_clicked();

    void manySendSlot(bool hexFlag,QByteArray data);

    void com_state_slots(bool comState);

    void on_readBtn_clicked();

private:
    Ui::MainWindow *ui;

    void initWindow();

    int sendByteNum;
    int receiveByteNum;
    //多项发送界面初始化
    ManySendDialog *manySendDialog;

    QMessageBox *msgBox;

};

#endif // MAINWINDOW_H
