#ifndef TCPCLIENTTHREAD_H
#define TCPCLIENTTHREAD_H

#include <QObject>

class QThread;
class tcpClientComm;

class tcpClientThread : public QObject
{
    Q_OBJECT
public:
    explicit tcpClientThread(QObject *parent = nullptr);
    ~tcpClientThread();
    bool start(QString localIP,quint16 localPort);

signals:
    void sig_connectSuccessMap();
    void sig_closeMap();
    void sig_recvMsgMap(QByteArray,QString,quint16);
    void sig_sendMsgMap(QByteArray, qint64);
    void sig_serverDisconnectMap();



private:
    QThread* m_thread;
    tcpClientComm* m_tcpClientComm;
};

#endif // TCPCLIENTTHREAD_H
