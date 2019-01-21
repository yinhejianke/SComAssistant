#ifndef DATALOGTHRAD_H
#define DATALOGTHRAD_H

#include <QThread>
#include <QByteArray>

class DataLogThread : public QThread
{
    Q_OBJECT
public:
    explicit DataLogThread();
    ~DataLogThread();

//    virtual void run();
    bool runFlag;

    QByteArray threadData;

public slots:
    void saveDataLog(QByteArray array, bool dataStream, QString dataSource);

};

#endif // DATALOGTHRAD_H
