#ifndef MANYSENDDIALOG_H
#define MANYSENDDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QByteArray>
#include <QTimer>

namespace Ui {
class ManySendDialog;
}

typedef struct hexSendFlag{
    bool hexFlag0;
    bool hexFlag1;
    bool hexFlag2;
    bool hexFlag3;
    bool hexFlag4;
    bool hexFlag5;
    bool hexAllFlag;
}hexSend;

class ManySendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManySendDialog(QWidget *parent = 0);
    ~ManySendDialog();

    void keyBtn(int keyNum);
    void keyCBox(int keyNum);

    void dataDeal();

signals:

    void sendDataSignal(bool,QByteArray);

private slots:

    void getKeySlot();
    void timeOutSlot();


private:
    Ui::ManySendDialog *ui;
    hexSend mySendHex;
    QTimer *sendTime;
    int sendNum;
    int sendAllNum;
    QByteArray myByteData;

};

#endif // MANYSENDDIALOG_H
