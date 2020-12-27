#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include "serialcomm.h"

//class serialComm;
class QThread;
class serialThread : public QObject
{
    Q_OBJECT
public:
    explicit serialThread(QObject *parent = nullptr);
    ~serialThread();

    bool start(QString _portName,int _baudRateInx,int _dataBitInx,
               int _stopBitInx,int _parityInx,int _flowControlInx);


signals:
    void sig_close();
    void sig_recvMsgMap(QByteArray);
    void sig_sendMsg(QByteArray);

private:
    serialComm* m_serialComm;
    QThread* m_thread;


};

#endif // SERIALTHREAD_H
