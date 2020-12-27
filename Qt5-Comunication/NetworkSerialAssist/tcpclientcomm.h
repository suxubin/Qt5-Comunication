#ifndef TCPCLIENTCOMM_H
#define TCPCLIENTCOMM_H

#include <QTcpSocket>

class tcpClientComm : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpClientComm(QObject *parent = nullptr);

    bool start(QString localIP,quint16 localPort);

signals:
    void sig_recvMsg(QByteArray,QString,quint16);


public slots:
    void slot_recvHandler();
    void slot_close();
    void slot_sendMsg(QByteArray buf,qint64 bufSize);



};

#endif // TCPCLIENTCOMM_H
