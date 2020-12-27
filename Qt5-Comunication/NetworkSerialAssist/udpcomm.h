#ifndef UDPCOMM_H
#define UDPCOMM_H


#include <QUdpSocket>

class udpComm : public QUdpSocket
{
    Q_OBJECT
public:
    explicit udpComm(QObject *parent = nullptr);
    bool start(QString localIp,quint16 localPort);


signals:
    void sig_recvMsg(QByteArray,QString,quint16);


public slots:
    void slot_recvHandler();
    void slot_close();
    void slot_sendMsg(QByteArray buf,QString remoteIP,quint16 remotePort);
    //void slot_error(QAbstractSocket::SocketError);

};

#endif // UDPCOMM_H
