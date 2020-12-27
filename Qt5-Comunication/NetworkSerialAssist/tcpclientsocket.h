#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>


class tcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClientSocket(qintptr socketDescriptor, QObject *parent = Q_NULLPTR);

signals:
    void sig_recvMsg(qintptr,QByteArray);
    void sig_clientDisConnect(qintptr,QString,quint16);
    //void sig_connectSuccess(QString,quint16);
    void sig_close();
    void sig_sendMsg(QByteArray,int);


public slots:


private:
    qintptr m_socketId;
    QString m_remoteIp;
    quint16 m_remotePort;

};

#endif // TCPCLIENTSOCKET_H
