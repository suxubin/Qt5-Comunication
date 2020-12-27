#include "udpcomm.h"

udpComm::udpComm(QObject *parent) : QUdpSocket(parent)
{

}

bool udpComm::start(QString localIp, quint16 localPort)
{

    if(localIp==""||localPort<=0)
        return false;

    if(!bind(QHostAddress(localIp),localPort,
             BindFlag::ShareAddress|BindFlag::ReuseAddressHint))
        return false;

    connect(this,&QUdpSocket::readyRead,
        this,&udpComm::slot_recvHandler);

    return true;

}

void udpComm::slot_recvHandler()
{
    while(hasPendingDatagrams())
    {
        QHostAddress remoteIp("127.0.0.1");
        quint16 remotePort=0;
        QByteArray buf;
        buf.resize(pendingDatagramSize());

        readDatagram(buf.data(),buf.size(),&remoteIp,&remotePort);

        emit sig_recvMsg(buf,remoteIp.toString(),remotePort);

    }

}

void udpComm::slot_close()
{
    close();

}

void udpComm::slot_sendMsg(QByteArray buf, QString remoteIP, quint16 remotePort)
{
    if(state()==QUdpSocket::SocketState::BoundState)
         writeDatagram(buf,QHostAddress(remoteIP),remotePort);
}


