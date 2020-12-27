#include "tcpclientthread.h"
#include "tcpclientcomm.h"
#include <QThread>

tcpClientThread::tcpClientThread(QObject *parent) : QObject(parent)
{
    m_tcpClientComm=nullptr;
    m_thread=nullptr;
}

tcpClientThread::~tcpClientThread()
{
    if(m_tcpClientComm){
        delete m_tcpClientComm;
        m_tcpClientComm=nullptr;
    }
}



bool tcpClientThread::start(QString localIP, quint16 localPort)
{

    m_tcpClientComm=new tcpClientComm;

    connect(m_tcpClientComm,&QTcpSocket::connected,
            this,&tcpClientThread::sig_connectSuccessMap);

    if(!m_tcpClientComm->start(localIP,localPort))
        return false;

    connect(this,&tcpClientThread::sig_closeMap,
            m_tcpClientComm,&tcpClientComm::slot_close);

    connect(m_tcpClientComm,&tcpClientComm::sig_recvMsg,
            this,&tcpClientThread::sig_recvMsgMap);

    connect(this,&tcpClientThread::sig_sendMsgMap,
            m_tcpClientComm,&tcpClientComm::slot_sendMsg);

    m_thread=new QThread(m_tcpClientComm);

    connect(m_tcpClientComm,&tcpClientComm::disconnected,
            m_thread,&QThread::quit);

    connect(m_tcpClientComm,&tcpClientComm::disconnected,
            this,&tcpClientThread::sig_serverDisconnectMap);

    connect(m_thread,&QThread::finished,
            this,&tcpClientThread::deleteLater);

    m_tcpClientComm->moveToThread(m_thread);
    m_thread->start();


}
