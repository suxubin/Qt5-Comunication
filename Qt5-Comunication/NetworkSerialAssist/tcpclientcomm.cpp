#include "tcpclientcomm.h"
#include <QHostAddress>

tcpClientComm::tcpClientComm(QObject *parent) : QTcpSocket(parent)
{

}

bool tcpClientComm::start(QString localIP, quint16 localPort)
{
    if(localIP==""||localPort<=0)
        return false;

    connect(this,&QTcpSocket::connected,
            [this](){
            connect(this,&QTcpSocket::readyRead,
                    this,&tcpClientComm::slot_recvHandler);
    });

    connectToHost(QHostAddress(localIP),localPort);
    if(!waitForConnected(100))//100ms
        return false;

    return true;

}

void tcpClientComm::slot_recvHandler()
{

    QByteArray buf;
//    int nSize=bytesAvailable();
//    if(nSize>0)
//        readData(buf.data(),nSize);

    //same function but testing simply
    buf=readAll();


    emit sig_recvMsg(buf,peerAddress().toString(),peerPort());


}

void tcpClientComm::slot_close()
{
    //close--->disConnectFromHost-->disConnected
    close();
}

void tcpClientComm::slot_sendMsg(QByteArray buf, qint64 bufSize)
{
    if(state()==QAbstractSocket::SocketState::ConnectedState)
        writeData(buf.data(),bufSize);


}


