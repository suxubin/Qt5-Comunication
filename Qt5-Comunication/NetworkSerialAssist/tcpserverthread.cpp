#include "tcpserverthread.h"
#include "tcpservercomm.h"
#include <QMetaType>
#include <QThread>
#include <QDebug>

Q_DECLARE_METATYPE(QString);
Q_DECLARE_METATYPE(quint16);
Q_DECLARE_METATYPE(qintptr);
Q_DECLARE_METATYPE(QByteArray);
tcpServerThread::tcpServerThread(QObject *parent) :QObject(parent)
{
    m_thread=nullptr;
    m_tcpServerComm=nullptr;
}

bool tcpServerThread::start(QString localIP, quint16 localPort)
{
    //main thread
    qRegisterMetaType<QByteArray>();
    qRegisterMetaType<QString>();
    qRegisterMetaType<quint16>();
    qRegisterMetaType<qintptr>("qintptr");

    qDebug()<<"mainThread"<<QThread::currentThread();
    m_tcpServerComm=new tcpServerComm;


    connect(m_tcpServerComm,&tcpServerComm::sig_connectSuccessMap,
            this,&tcpServerThread::sig_connectSuccessMap);


    if(!m_tcpServerComm->start(localIP,localPort))
        return false;

    connect(m_tcpServerComm,&tcpServerComm::sig_clientDisconnectMap,
            this,&tcpServerThread::sig_clientDisConnectMap);


    connect(m_tcpServerComm,&tcpServerComm::sig_recvMsgMap,
            this,&tcpServerThread::sig_recvMsgMap);


    connect(this,&tcpServerThread::sig_sendMsgMap,
            m_tcpServerComm,&tcpServerComm::sig_sendMsgMap);

    connect(this,&tcpServerThread::sig_sendMsgToAllClientMap,
            m_tcpServerComm,&tcpServerComm::sig_sendMsgToAllClient);

    connect(this,&tcpServerThread::sig_serverDisconnect,
            m_tcpServerComm,&tcpServerComm::sig_serverDisconnect);


//    connect(this,&tcpServerThread::sig_closeMap,
//            m_tcpServerComm,&tcpServerComm::sig_closeMap);

    connect(this,&tcpServerThread::sig_closeOneClient,
            m_tcpServerComm,&tcpServerComm::sig_closeOneClient);

    m_thread=new QThread(m_tcpServerComm);
    m_tcpServerComm->moveToThread(m_thread);

    connect(m_tcpServerComm,&tcpServerComm::sig_closeFinnish,
            m_thread,&QThread::quit);

    connect(m_thread,&QThread::finished,
            this,&tcpServerThread::deleteLater);

    m_thread->start();

    return true;

}


