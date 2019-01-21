#include "datalogthrad.h"
#include <QDebug>

#ifdef win32
#pragma execution_character_set("utf-8")
#endif


DataLogThread::DataLogThread()
{
    this->moveToThread(this);
}

DataLogThread::~DataLogThread()
{

}

//void DataLogThread::run()
//{
//    while (true)
//    {
//        this->exec();
//        DataLog::saveData(threadData,true,"接受");
//        threadData.clear();
//    }
//}

void DataLogThread::saveDataLog(QByteArray array, bool dataStream, QString dataSource)
{
    // DataLog::saveData(array,dataStream,dataSource);
}
