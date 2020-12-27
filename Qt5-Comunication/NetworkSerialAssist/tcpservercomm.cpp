#include "tcpservercomm.h"
#include "tcpclientsocket.h"
#include <QTimer>

#include <QThread>
#include <QDebug>


Q_DECLARE_METATYPE(QString);
Q_DECLARE_METATYPE(quint16);
Q_DECLARE_METATYPE(qintptr);
Q_DECLARE_METATYPE(QByteArray);
tcpServerComm::tcpServerComm(QObject *parent) : QTcpServer(parent)
{

}

bool tcpServerComm::start(QString localIP, quint16 localPort)
{
    if(localIP==""||localPort<=0)
        return false;

    if(!listen(QHostAddress(localIP),localPort))
        return false;

    //-----------服务端主动暴力断开的情况---------
    connect(this,&tcpServerComm::sig_serverDisconnect,
            [this](){
                qDebug()<<"sig_serverDisconnect:"<<QThread::currentThread();
                for(auto itr=m_tcpClients.begin();
                    itr!=m_tcpClients.end();++itr)
                {
                    emit itr.value()->sig_close();

                }

                //如果需要处理多个客户端，可能会出现还没有完全退出就关闭监听和线程情况
                close(); //退出监听
                emit sig_closeFinnish();//退出线程
    });


    return true;



}

void tcpServerComm::incomingConnection(qintptr socketDescriptor)
{
    //serverThread register
    qRegisterMetaType<QByteArray>();
    qRegisterMetaType<QString>();
    qRegisterMetaType<quint16>();
    qRegisterMetaType<qintptr>("qintptr");

    qDebug()<<"serverThread:"<<QThread::currentThread();

    tcpClientSocket* tempClientSocket=new tcpClientSocket(socketDescriptor);

    //server thread
    emit sig_connectSuccessMap(tempClientSocket->peerAddress().toString(),
                               tempClientSocket->peerPort());

    QThread* tempThread=new QThread();

    connect(tempClientSocket,&tcpClientSocket::sig_clientDisConnect,this,
            [this](qintptr socketId,QString remoteIp,quint16 remotePort){

             qDebug()<<"sig_clientDisConnect-Thread:"<<QThread::currentThread();
                auto oneItem=m_tcpClients.find(socketId);
                if(oneItem!=m_tcpClients.end())
                {
                    //此时 socket已经断开，无法通过peerAddress()获取ip
//                    emit sig_clientDisconnectMap(oneItem.value()->peerAddress().toString(),
//                                                  oneItem.value()->peerPort());
                    emit sig_clientDisconnectMap(remoteIp,remotePort);
                    tcpClientSocket* temp=oneItem.value();
                    m_tcpClients.remove(socketId);
                    emit sig_clearOneClientFinish(socketId);
                    delete temp;temp=nullptr;
                }


      //指定由serverThread执行,默认是clientThread执行而导致竞争
    },Qt::QueuedConnection);//lambad,


    connect(this,&tcpServerComm::sig_clearOneClientFinish,this,
            [this](qintptr socketId){
            qDebug()<<"sig_clearOneClientFinish-Thread:"<<QThread::currentThread();
            auto oneThread=m_tcpClientThreads.find(socketId);
            if(oneThread!=m_tcpClientThreads.end()){
                QThread* temp=oneThread.value();
                m_tcpClientThreads.remove(socketId);

                //由系统自行释放，且由clientThread自己释放自己
                connect(temp,&QThread::finished,temp,&QObject::deleteLater);

                /*connect(temp,&QThread::finished,this,
                        [&temp](){
                    qDebug()<<"clientThread:"<<QThread::currentThreadId();
                    delete temp;temp=nullptr;},Qt::QueuedConnection);*///lambad inSide

                temp->wait(1000);
                /*if(temp&&temp->isRunning())
                    temp->quit();*///called exit powerfully

            }//if

    });//lambad outSide


    connect(tempClientSocket,&tcpClientSocket::sig_recvMsg,this,
            [this](qintptr socketId,QByteArray buf){
                 auto oneItem=m_tcpClients.find(socketId);
                 qDebug()<<"sig_recvMsg-Thread:"<<QThread::currentThread();
                 if(oneItem!=m_tcpClients.end())
                    emit sig_recvMsgMap(buf,oneItem.value()->peerAddress().toString(),
                                        oneItem.value()->peerPort());
    },Qt::QueuedConnection);//指定由serverThread执行，与写操作同属一个线程

    connect(this,&tcpServerComm::sig_closeOneClient,
            [this](QString ip,quint16 port){
             for(auto oneItem=m_tcpClients.begin();
                 oneItem!=m_tcpClients.end();++oneItem){
                 tcpClientSocket* temp=oneItem.value();
                 if(temp->peerAddress().toString()==ip&&
                    temp->peerPort()==port){
                     emit temp->sig_close();
                 }
             }

    });


//    connect(this,&tcpServerComm::sig_closeMap,
//            tempClientSocket,&tcpClientSocket::sig_close);

    connect(this,&tcpServerComm::sig_sendMsgMap,
            [this](QString clientIp, quint16 clientPort,QByteArray buf,int bufSize){
            qDebug()<<"sig_sendMsgMap-Thread:"<<QThread::currentThread();
            for(auto itr=m_tcpClients.begin();
                itr!=m_tcpClients.end();++itr)
            {
                //和 c++不一样？ itr在这里是某一项对象，而不是指向某一项的指针
                if (itr.value()->peerAddress().toString()==clientIp&&
                       itr.value()->peerPort()==clientPort)
                {
                    emit itr.value()->sig_sendMsg(buf,bufSize);

                }
            }

           });//labdam


    connect(this,&tcpServerComm::sig_sendMsgToAllClient,
            [this](QByteArray buf,int bufSize){
            qDebug()<<"sig_sendMsgToAllClient-Thread:"<<QThread::currentThread();
            for(auto itr=m_tcpClients.begin();
                itr!=m_tcpClients.end();++itr)
            {
                emit itr.value()->sig_sendMsg(buf,bufSize);

            }
    });


    tempClientSocket->moveToThread(tempThread);
    tempThread->start();

    m_tcpClients.insert(socketDescriptor,tempClientSocket);
    m_tcpClientThreads.insert(socketDescriptor,tempThread);

}
