#include "serialthread.h"

#include <QThread>

serialThread::serialThread(QObject *parent) : QObject(parent)
{
    m_serialComm=nullptr;
    m_thread=nullptr;
}

serialThread::~serialThread()
{
    if(m_serialComm){
        delete m_serialComm;
        m_serialComm=nullptr;
    }

    //不需要，因为其父类是m_serialComm;
//    if(m_thread){
//        delete m_thread;
//        m_thread=nullptr;
//    }


}

bool serialThread::start(QString _portName, int _baudRateInx, int _dataBitInx,
                         int _stopBitInx, int _parityInx, int _flowControlInx)
{

    m_serialComm=new serialComm;

    m_thread=new QThread(m_serialComm);

    if(!m_serialComm->start(_portName,_baudRateInx,_dataBitInx,
                        _stopBitInx,_parityInx,_flowControlInx))
        return false;

    connect(m_serialComm,&serialComm::sig_recvMsg,
            this,&serialThread::sig_recvMsgMap);

    connect(this,&serialThread::sig_sendMsg,
            m_serialComm,&serialComm::slot_sendMsg);

    connect(this,&serialThread::sig_close,
            m_serialComm,&serialComm::slot_close);

    connect(m_serialComm,&serialComm::sig_closeFinish,
            m_thread,&QThread::quit);

    connect(m_thread,&QThread::finished,
            this,&serialThread::deleteLater);


    m_serialComm->moveToThread(m_thread);
    m_thread->start();

    return true;


}



