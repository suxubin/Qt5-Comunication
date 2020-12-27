#ifndef TCPSERVERTHREAD_H
#define TCPSERVERTHREAD_H

#include <QObject>

class QThread;
class tcpServerComm;
class tcpServerThread : public QObject
{
    Q_OBJECT
public:
    explicit tcpServerThread(QObject *parent = nullptr);

    bool start(QString localIP,quint16 localPort);

signals:
    void sig_closeOneClient(QString,quint16);
    void sig_clearOneClientFinish(qintptr);

    //void sig_closeMap();
    void sig_closeFinnish();

    void sig_serverDisconnect();

    void sig_connectSuccessMap(QString,quint16);
    void sig_clientDisConnectMap(QString,quint16);

    void sig_recvMsgMap(QByteArray,QString,quint16);
    void sig_sendMsgMap(QString,quint16,QByteArray,int);
    void sig_sendMsgToAllClientMap(QByteArray ,int);


private:
    tcpServerComm* m_tcpServerComm;
    QThread* m_thread;

};

#endif // TCPSERVERTHREAD_H
