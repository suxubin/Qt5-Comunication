#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QObject>

class QThread;
class udpComm;
class udpThread : public QObject
{
    Q_OBJECT
public:
    explicit udpThread(QObject *parent = nullptr);
    ~udpThread();
    bool start(QString localIp,quint16 localPort);
signals:
    void sig_closeMap();
    void sig_recvMsgMap(QByteArray,QString,quint16);
    void sig_sendMsg(QByteArray,QString,quint16);


public slots:

private:
    QThread* m_thread;
    udpComm* m_udpComm;


};

#endif // UDPTHREAD_H
