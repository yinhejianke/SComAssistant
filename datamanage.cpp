#include "datamanage.h"

#ifdef win32
#pragma execution_character_set("utf-8")
#endif

DataManage::DataManage()
{

    this->moveToThread(this);

    qRegisterMetaType<myComInfo>("myComInfo");

    myComControl.changeFlag = false;
    myComControl.hexSendFlag = false;
    myComControl.hexShowFlag = false;
    myComControl.sendClearFlag = false;

}

DataManage::~DataManage()
{
    delete myCom;
}

//线程run函数
void DataManage::run()
{
    myCom = new QSerialPort();
    connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));
    while(true)
    {
        if(!receiveData.isEmpty())
        {
            dataAnalysis(receiveData);

            receiveData.clear();
        }
        this->exec();
    }
}
//数据分析
void DataManage::dataAnalysis(QByteArray data)
{
    int length = 0;
    int sendData = 0;


    if(0x55 == (unsigned char)data[0] && 0xaa == (unsigned char)data[1])//数据帧头
    {
        length = data.length();
        if(0xaa == (unsigned char)data[length-2] && 0x55 == (unsigned char)data[length-1])
        {
            sendData = (char)data[2]*256;
            sendData += (unsigned char)data[3];
            emit this->plotShowSignal(sendData);
        }
    }
    else if(0x44 == (unsigned char)data[0] && 0xbb == (unsigned char)data[1])//数据帧头
    {
        length = data.length();
        if(0xbb == (unsigned char)data[length-2] && 0x44 == (unsigned char)data[length-1])
        {
            myDataType myDatax,myDatay;
            for(int i=0;i<4;i++)
            {
                myDatax.int8_t[i] = data[i+2];
                myDatay.int8_t[i] = data[i+6];
            }
            emit this->XYdrawSignal(myDatax.f32_t[0],myDatay.f32_t[0]);
        }
    }
}
//打开关闭串口槽函数
void DataManage::openCloseSlot(myComInfo myComInfor)
{
//    qDebug()<<"openCloseSlot"<< this->thread()->currentThreadId();

    myCom->setPortName(myComInfor.portQString);
    if(myComInfor.baudQString == "9600")
        myCom->setBaudRate(QSerialPort::Baud9600);
    else if(myComInfor.baudQString == "115200")
        myCom->setBaudRate(QSerialPort::Baud115200);
    else if(myComInfor.baudQString == "1200")
        myCom->setBaudRate(QSerialPort::Baud1200);
    else if(myComInfor.baudQString == "2400")
        myCom->setBaudRate(QSerialPort::Baud2400);
    else if(myComInfor.baudQString == "4800")
        myCom->setBaudRate(QSerialPort::Baud4800);
    else if(myComInfor.baudQString == "19200")
        myCom->setBaudRate(QSerialPort::Baud19200);
    else if(myComInfor.baudQString == "38400")
        myCom->setBaudRate(QSerialPort::Baud38400);
    else if(myComInfor.baudQString == "57600")
        myCom->setBaudRate(QSerialPort::Baud57600);
    else if(myComInfor.baudQString == "194000")
        myCom->setBaudRate(QSerialPort::Baud194000);

    if(myComInfor.dataQString == "8")
        myCom->setDataBits(QSerialPort::Data8);
    else if(myComInfor.dataQString == "7")
        myCom->setDataBits(QSerialPort::Data7);
    else if(myComInfor.dataQString == "6")
        myCom->setDataBits(QSerialPort::Data6);
    else if(myComInfor.dataQString == "5")
        myCom->setDataBits(QSerialPort::Data5);

    if(myComInfor.parityQString == "无")
        myCom->setParity(QSerialPort::NoParity);
    else if(myComInfor.parityQString == "奇")
        myCom->setParity(QSerialPort::OddParity);
    else if(myComInfor.parityQString == "偶")
        myCom->setParity(QSerialPort::EvenParity);

    if(myComInfor.stopQString == "1")
        myCom->setStopBits(QSerialPort::OneStop);
    else if(myComInfor.stopQString == "1.5")
        myCom->setStopBits(QSerialPort::OneAndHalfStop);
    else if(myComInfor.stopQString == "2")
        myCom->setStopBits(QSerialPort::TwoStop);
    myCom->setFlowControl(QSerialPort::NoFlowControl);
    if("open" == myComInfor.stateQString)
        emit this->comStateSignal(myCom->open(QIODevice::ReadWrite));
    else
        myCom->close();
    this->exit();
}
//读取串口数据
void DataManage::readMyCom()
{
    receiveData = myCom->readAll();

    if(myComControl.hexShowFlag)
        emit this->receiveDataSignal(ByteArrayToHexStr(receiveData).toLocal8Bit());
    else
        emit this->receiveDataSignal(receiveData);

    emit this->saveDataSignal(receiveData,true,"接受");

    this->exit();
}
//发送数据槽函数
void DataManage::sendDataSlot(bool hexFlag,QByteArray data)
{
    //myComControl.hexSendFlag = hexFlag;
    if(hexFlag)
    {
        QByteArray temp = QString2Hex(data);
        emit this->saveDataSignal(temp,false,"发送");
        myCom->write(temp);
    }
    else
    {
        // DataLog::saveData(data,false,"发送");
        myCom->write(data);
    }
    this->exit();
}
//hex显示槽函数
void DataManage::hexShowSlot(bool hexFlag)
{
    myComControl.hexShowFlag = hexFlag;
    this->exit();
}
//Hex发送函数
char DataManage::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}
QByteArray DataManage::QString2Hex(QString str)
{
    QByteArray senddata;
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
    return senddata;
}
//字节数组转换为16进制字符串
QString DataManage::ByteArrayToHexStr(QByteArray data)
{
    QString temp="";
    QString hex=data.toHex();
    for (int i=0;i<hex.length();i=i+2)
    {
        temp+=hex.mid(i,2)+" ";
    }
    return temp.trimmed().toUpper();
}

