#ifndef TCPSERVERCOMM_H
#define TCPSERVERCOMM_H

#include <QTcpServer>
#include <QHash>

class tcpClientSocket;
class QThread;
class tcpServerComm : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServerComm(QObject *parent = nullptr);

     bool start(QString localIP,quint16 localPort);
     virtual void incomingConnection(qintptr socketDescriptor);

signals:
     void sig_connectSuccessMap(QString,quint16);
     //void sig_closeMap();
     void sig_closeOneClient(QString,quint16);


     void sig_clientDisconnectMap(QString,quint16);
     void sig_clearOneClientFinish(qintptr);

     void sig_serverDisconnect();
     void sig_closeFinnish();

     void sig_recvMsgMap(QByteArray,QString,quint16);

     void sig_sendMsgMap(QString,quint16,QByteArray,int);
     void sig_sendMsgToAllClient(QByteArray ,int);


private:
     QHash<qintptr,tcpClientSocket*> m_tcpClients;
     QHash<qintptr,QThread*> m_tcpClientThreads;

};

#endif // TCPSERVERCOMM_H
