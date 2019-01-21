#include "manysenddialog.h"
#include "ui_manysenddialog.h"

#ifdef win32
#pragma execution_character_set("utf-8")
#endif

ManySendDialog::ManySendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManySendDialog)
{
    ui->setupUi(this);

    sendNum = 0;
    sendAllNum = 0;

    mySendHex.hexFlag0 = false;
    mySendHex.hexFlag1 = false;
    mySendHex.hexFlag2 = false;
    mySendHex.hexFlag3 = false;
    mySendHex.hexFlag4 = false;
    mySendHex.hexFlag5 = false;
    mySendHex.hexAllFlag = false;

    ui->sendBtn0->setWhatsThis("0");
    ui->sendBtn1->setWhatsThis("1");
    ui->sendBtn2->setWhatsThis("2");
    ui->sendBtn3->setWhatsThis("3");
    ui->sendBtn4->setWhatsThis("4");
    ui->sendBtn5->setWhatsThis("5");
    ui->manySendBtn->setWhatsThis("6");

    ui->hexCBox0->setWhatsThis("0");
    ui->hexCBox1->setWhatsThis("1");
    ui->hexCBox2->setWhatsThis("2");
    ui->hexCBox3->setWhatsThis("3");
    ui->hexCBox4->setWhatsThis("4");
    ui->hexCBox5->setWhatsThis("5");
    ui->hexAllCBox->setWhatsThis("6");

    connect(ui->sendBtn0,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->sendBtn1,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->sendBtn2,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->sendBtn3,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->sendBtn4,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->sendBtn5,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexCBox0,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexCBox1,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexCBox2,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexCBox3,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexCBox4,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexCBox5,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->hexAllCBox,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));
    connect(ui->manySendBtn,SIGNAL(clicked(bool)),this,SLOT(getKeySlot()));

    this->sendTime = NULL;
    this->sendTime = new QTimer();
    connect(sendTime,SIGNAL(timeout()),this,SLOT(timeOutSlot()));
    this->sendTime->stop();
}

ManySendDialog::~ManySendDialog()
{
    this->sendTime->stop();
    this->sendTime->deleteLater();
    delete sendTime;
    delete ui;
}
//获取按键编号
void ManySendDialog::getKeySlot()
{
    QString myBtn,myCtn;
    int keyNum;
    bool ok=false;
    if (QPushButton* btn = dynamic_cast<QPushButton*>(sender()))
    {
        myBtn = btn->whatsThis();
        keyNum = myBtn.toInt(&ok,10);
        keyBtn(keyNum);
    }
    else if (QCheckBox *ctn = dynamic_cast<QCheckBox*>(sender()))
    {
        myCtn = ctn->whatsThis();
        keyNum = myCtn.toInt(&ok,10);
        keyCBox(keyNum);
    }
}
//对应按钮槽函数
void ManySendDialog::keyBtn(int keyNum)
{
    bool ok = false;
    switch (keyNum) {
start:
    case 0:
        if(!ui->lineEdit0->text().isEmpty())
        {
            emit this->sendDataSignal(mySendHex.hexFlag0,ui->lineEdit0->text().toLocal8Bit());
            break;
        }
        sendNum++;
    case 1:
        if(!ui->lineEdit1->text().isEmpty())
        {
            emit this->sendDataSignal(mySendHex.hexFlag1,ui->lineEdit1->text().toLocal8Bit());
            break;
        }
        sendNum++;
    case 2:
        if(!ui->lineEdit2->text().isEmpty())
        {
            emit this->sendDataSignal(mySendHex.hexFlag2,ui->lineEdit2->text().toLocal8Bit());
            break;
        }
        sendNum++;
    case 3:
        if(!ui->lineEdit3->text().isEmpty())
        {
            emit this->sendDataSignal(mySendHex.hexFlag3,ui->lineEdit3->text().toLocal8Bit());
            break;
        }
        sendNum++;
    case 4:
        if(!ui->lineEdit4->text().isEmpty())
        {
            emit this->sendDataSignal(mySendHex.hexFlag4,ui->lineEdit4->text().toLocal8Bit());
            break;
        }
        sendNum++;
    case 5:
        if(!ui->lineEdit5->text().isEmpty())
        {
            emit this->sendDataSignal(mySendHex.hexFlag5,ui->lineEdit5->text().toLocal8Bit());
            break;
        }
        else if(sendAllNum > 1)
        {
            sendNum = 0;
            sendAllNum--;
            goto start;
        }
        break;
    //循环发送
    case 6:
        if(1 == ui->tabWidget->currentIndex())
        {
            myByteData = ui->plainTextEdit->toPlainText().toLocal8Bit();
            myByteData.append('\n');
        }
        sendTime->start(ui->sendTimeLEdit->text().toInt(&ok,10));
        sendAllNum = ui->sendNumLEdit->text().toInt(&ok,10);
        break;
    default:
        break;
    }
}
//对应hex
void ManySendDialog::keyCBox(int keyNum)
{
    switch (keyNum) {
    case 0:
        if(ui->hexCBox0->isChecked())
            mySendHex.hexFlag0 = true;
        else
            mySendHex.hexFlag0 = false;
        break;
    case 1:
        if(ui->hexCBox0->isChecked())
            mySendHex.hexFlag1 = true;
        else
            mySendHex.hexFlag1 = false;
        break;
    case 2:
        if(ui->hexCBox0->isChecked())
            mySendHex.hexFlag2 = true;
        else
            mySendHex.hexFlag2 = false;
        break;
    case 3:
        if(ui->hexCBox0->isChecked())
            mySendHex.hexFlag3 = true;
        else
            mySendHex.hexFlag3 = false;
        break;
    case 4:
        if(ui->hexCBox0->isChecked())
            mySendHex.hexFlag4 = true;
        else
            mySendHex.hexFlag4 = false;
        break;
    case 5:
        if(ui->hexCBox0->isChecked())
            mySendHex.hexFlag5 = true;
        else
            mySendHex.hexFlag5 = false;
        break;
    case 6:
        if(ui->hexAllCBox->isChecked())
            mySendHex.hexAllFlag = true;
        else
            mySendHex.hexAllFlag = false;
        break;
    default:
        break;
    }
}

void ManySendDialog::timeOutSlot()
{
    if(0 == ui->tabWidget->currentIndex())
    {
        keyBtn(sendNum);
        sendNum++;
        if(sendNum >= 6)
        {
            sendNum = 0;
            sendAllNum--;
            if(0 == sendAllNum)
            {
                sendTime->stop();
            }
        }
    }
    else if(1 == ui->tabWidget->currentIndex())
    {
        dataDeal();
    }
}

void ManySendDialog::dataDeal()
{
    static int piont = 0;
    int myLength = 0;
    QByteArray ByteSend;
    myLength = myByteData.length();
    if(!myByteData.isEmpty())
    {
        for(int i=piont;i<myLength;i++)
        {
            //每帧回车分割
            if('\n' == myByteData[i])
            {
                ByteSend = myByteData.mid(piont,i-piont);
                emit this->sendDataSignal(mySendHex.hexAllFlag,ByteSend);
                piont = i+1;
                //最后一帧
                if(piont == myLength)
                {
                    piont = 0;
                    sendAllNum--;
                    if(0 == sendAllNum)
                    {
                        sendTime->stop();
                    }
                }
                break;
            }
        }
    }
}

