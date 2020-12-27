#include "udpthread.h"
#include "udpcomm.h"
#include <QThread>

udpThread::udpThread(QObject *parent) : QObject(parent)
{
    m_thread=nullptr;
    m_udpComm=nullptr;
}

udpThread::~udpThread()
{
    if(m_udpComm){
        delete m_udpComm;
        m_udpComm=nullptr;
    }
}




bool udpThread::start(QString localIp, quint16 localPort)
{

    m_udpComm=new udpComm;
    if(!m_udpComm->start(localIp,localPort))
        return false;

    connect(m_udpComm,&udpComm::sig_recvMsg,
            this,&udpThread::sig_recvMsgMap);

    connect(this,&udpThread::sig_sendMsg,
            m_udpComm,&udpComm::slot_sendMsg);

    connect(this,&udpThread::sig_closeMap,
            m_udpComm,&udpComm::slot_close);

    m_thread=new QThread(m_udpComm);
    m_udpComm->moveToThread(m_thread);

    connect(m_udpComm,&udpComm::disconnected,
            m_thread,&QThread::quit);

    connect(m_thread,&QThread::finished,
            this,&udpThread::deleteLater);

    m_thread->start();
    return true;

}





