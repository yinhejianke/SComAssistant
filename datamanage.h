#ifndef DATAMANAGE_H
#define DATAMANAGE_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QMetaType>
#include <QMessageBox>

typedef struct comInfo{
    QString stateQString;
    QString portQString;
    QString baudQString;
    QString dataQString;
    QString stopQString;
    QString parityQString;
}myComInfo;

typedef struct comControl{
    bool hexShowFlag;   //hex显示标志
    bool changeFlag;    //帧换行
    bool hexSendFlag;   //hex发送
    bool sendClearFlag; //发送清空
}myComCon;

typedef union dataType{
    char int8_t[8];
    unsigned char uint8_t[8];
    short int int16_t[4];
    unsigned short uint16_t[4];
    int int32_t[2];
    unsigned int uint32_t[2];
    float f32_t[2];
    double d64_t;
}myDataType;

class DataManage : public QThread
{
    Q_OBJECT
public:
    explicit DataManage();
    ~DataManage();

    virtual void run();
    void dataAnalysis(QByteArray data);

private:
    QSerialPort *myCom;
    QString portString;
    QString boadradeString;
    QByteArray receiveData;
    myComCon myComControl;

    char ConvertHexChar(char ch);
    QByteArray QString2Hex(QString str);
    QString ByteArrayToHexStr(QByteArray data);


signals:
    void receiveDataSignal(QByteArray);
    void saveDataSignal(QByteArray, bool, QString);
    void plotShowSignal(int);
    void XYdrawSignal(double,double);
    void XYdrawSignal(float,float);
    void comStateSignal(bool);


public slots:


private slots:
    void readMyCom();
    void sendDataSlot(bool hexFlag, QByteArray data);
    void openCloseSlot(myComInfo myComInfor);
    void hexShowSlot(bool hexFlag);

};

#endif // DATAMANAGE_H
