#include "tcpclientsocket.h"
#include <QHostAddress>

#include <QDebug>
#include <QThread>

Q_DECLARE_METATYPE(QString);
Q_DECLARE_METATYPE(quint16);
Q_DECLARE_METATYPE(qintptr);
Q_DECLARE_METATYPE(QByteArray);
tcpClientSocket::tcpClientSocket(qintptr socketDescriptor, QObject *parent)
    :QTcpSocket(parent),m_socketId(socketDescriptor)
{
    setSocketDescriptor(socketDescriptor);

    //主要是由于，在close下将ip/port信息送回Gui
    m_remoteIp=peerAddress().toString();
    m_remotePort=peerPort();

//     qDebug()<<"serverThread:"<<QThread::currentThreadId();

    connect(this,&QTcpSocket::readyRead,
            [this](){
        QByteArray buf;
        buf=readAll();

        emit sig_recvMsg(m_socketId,buf);

    });

//    connect(this,&QTcpSocket::connected,
//            [this](){
//            qRegisterMetaType<QByteArray>();
//            qRegisterMetaType<QString>();
//            qRegisterMetaType<quint16>();
//            qRegisterMetaType<qintptr>();

//            qDebug()<<"clientThread:"<<QThread::currentThreadId();

//            emit sig_connectSuccess(peerAddress().toString(),peerPort());

//    });


    connect(this,&QTcpSocket::disconnected,
            [this](){
            qDebug()<<"clientThread:"<<QThread::currentThread();
            //qRegisterMetaType<qintptr>();
            emit sig_clientDisConnect(m_socketId,m_remoteIp,m_remotePort);
    });


    //defalut direConnect 由发射线程serverThread执行
    connect(this,&tcpClientSocket::sig_close,this,
            [this](){
            qDebug()<<"sig_close-Thread:"<<QThread::currentThread();
            close(); //close io-->disconnectFromHost();
    },Qt::QueuedConnection);//指定是clientthread执行

    connect(this,&tcpClientSocket::sig_sendMsg,this,
            [this](QByteArray buf,int bufSize){
            qDebug()<<"tcpClientSocket::sig_sendMsg-thread:"<<QThread::currentThread();
            if(state()==QAbstractSocket::SocketState::ConnectedState)
                writeData(buf.data(),bufSize);
    },Qt::QueuedConnection);//指定由clientThread自己执行



}

