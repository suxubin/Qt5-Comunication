#include "tcpservergui.h"
#include <QSplitter>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextBrowser>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QSpinBox>
#include <QMetaEnum>
#include <QMessageBox>
#include <QTimer>
#include <QHostInfo>
#include <QDateTime>
#include <QHostAddress>
#include <QAbstractSocket>

#include "tcpserverthread.h"

tcpServerGui::tcpServerGui(QWidget *parent)
    : QWidget(parent),m_tcpServerThread(nullptr),m_timer(nullptr)
{

    QPalette mainPalette;
    mainPalette.setColor(QPalette::Background,QColor(239,239,239));
    this->setAutoFillBackground(true);
    this->setPalette(mainPalette);


    QVBoxLayout* mainLayout=new QVBoxLayout(this);

    QSplitter* mainSplitter=new QSplitter(Qt::Horizontal);

    m_wigLeft=new QWidget(mainSplitter);
    QWidget* rightWig=new QWidget(mainSplitter);

    QHBoxLayout* rightLayout=new QHBoxLayout(rightWig);
    QSplitter* rightSplitter=new QSplitter(Qt::Vertical);

    m_wigRightUp=new QWidget(rightSplitter);
    m_wigRightBottom=new QWidget(rightSplitter);
    rightSplitter->setStretchFactor(0,4);
    rightSplitter->setStretchFactor(1,1);

    rightLayout->addWidget(rightSplitter);

    mainSplitter->setStretchFactor(0,0);
    mainSplitter->setStretchFactor(1,4);
    mainSplitter->setHandleWidth(2);

    mainLayout->addWidget(mainSplitter);

    createLeftWig();
    CreateRightWig();

}

void tcpServerGui::createLeftWig()
{
    m_leftLayout=new QVBoxLayout(m_wigLeft);
    m_leftLayout->setMargin(0);
    createTcpSetting();
    createRecvSetting();
    createSendSetting();

}
void tcpServerGui::createTcpSetting()
{
    QGroupBox* tcpGB=new QGroupBox;
    tcpGB->setFlat(true);
    tcpGB->setTitle("网络设置");
    QGridLayout* netWorkLayout=new QGridLayout;

    int lines=0;

    QLabel* localIp=new QLabel("服务器ip");
    netWorkLayout->addWidget(localIp,lines,0);
    m_cbLocalIp=new QComboBox;
    QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
    foreach(auto ipAddr,info.addresses()){
        if(ipAddr.protocol()==QAbstractSocket::IPv4Protocol)
            m_cbLocalIp->addItem(ipAddr.toString());
    }
    netWorkLayout->addWidget(m_cbLocalIp,lines,1);


    QLabel *localPort=new QLabel("服务器端口");
    netWorkLayout->addWidget(localPort,++lines,0);
    m_leLocalPort=new QLineEdit();
    m_leLocalPort->setText("8080");
    m_leLocalPort->setMaximumWidth(m_cbLocalIp->width());
    netWorkLayout->addWidget(m_leLocalPort,lines,1);

    QLabel* tip=new QLabel("连接/断开");
    m_btnConnect=new QPushButton("连接");
    m_btnConnect->setCheckable(true);
    connect(m_btnConnect,&QPushButton::toggled,
            this,&tcpServerGui::slot_connectOrNot);
    netWorkLayout->addWidget(tip,++lines,0);
    netWorkLayout->addWidget(m_btnConnect,lines,1);

    tcpGB->setLayout(netWorkLayout);
    m_leftLayout->addWidget(tcpGB);

}
void tcpServerGui::createRecvSetting()
{
    QGroupBox* recvGB=new QGroupBox;
    recvGB->setTitle("接收设置");
    recvGB->setFlat(true);

    QVBoxLayout* recvLayout=new QVBoxLayout;

    m_cbCodeHex=new QCheckBox("十六进制接收");
    m_cbShowSendMsg=new QCheckBox("显示发送");
    m_cbShowTime=new QCheckBox("显示时间");
    m_btnClearRecvZone=new QPushButton("清空接收区");
    m_btnClearRecvZone->setFlat(true);
    m_btnClearRecvZone->setStyleSheet("color:rgb(0,112,224)");
    connect(m_btnClearRecvZone,&QPushButton::clicked,
            [this](){
            m_tbRecvText->clear();
    });

    recvLayout->addWidget(m_cbCodeHex);
    recvLayout->addWidget(m_cbShowSendMsg);
    recvLayout->addWidget(m_cbShowTime);
    recvLayout->addWidget(m_btnClearRecvZone);

    recvGB->setLayout(recvLayout);
    m_leftLayout->addWidget(recvGB);

}
void tcpServerGui::createSendSetting()
{
    QGroupBox* sendGB=new QGroupBox;
    sendGB->setTitle("发送设置");
    sendGB->setFlat(true);

    QVBoxLayout* sendLayout=new QVBoxLayout;


    m_cbSCodeHex=new QCheckBox("十六进制发送");
    connect(m_cbSCodeHex,&QCheckBox::stateChanged,
            [this](int state){
            QString resMsg=m_teSendText->toPlainText();
            //hex string-->string
            if(state!=Qt::CheckState::Checked){
                QByteArray buf=resMsg.toLatin1();
                resMsg=QString(buf.fromHex(buf));
            }
            //string-->hex string
            else{
                resMsg=QString(resMsg.toLatin1().toHex(' '));
            }

            m_teSendText->clear();
            m_teSendText->setText(resMsg);

    });
    sendLayout->addWidget(m_cbSCodeHex);

    QHBoxLayout* repeatLayout=new QHBoxLayout;
    m_sbRepeatTimeInterval=new QSpinBox;
    m_sbRepeatTimeInterval->setMaximum(50000);//50s
    m_sbRepeatTimeInterval->setSingleStep(10);//10ms
    m_sbRepeatTimeInterval->setValue(1000);//defalut 1s
    QLabel* tip=new QLabel("ms");
    m_cbRepeat=new QCheckBox("定时发送");
    connect(m_cbRepeat,&QCheckBox::stateChanged,
            this,&tcpServerGui::slot_timeOut);

    repeatLayout->addWidget(m_cbRepeat);
    repeatLayout->addSpacing(5);
    repeatLayout->addWidget(m_sbRepeatTimeInterval);
    repeatLayout->addWidget(tip);
    sendLayout->addLayout(repeatLayout);

    m_btnClearSendZone=new QPushButton("清空发送区");
    m_btnClearSendZone->setFlat(true);
    m_btnClearSendZone->setStyleSheet("color:rgb(0,112,224)");
    connect(m_btnClearSendZone,&QPushButton::clicked,
            [this](){
            m_teSendText->clear();
    });
    sendLayout->addWidget(m_btnClearSendZone);


    sendGB->setLayout(sendLayout);
    m_leftLayout->addWidget(sendGB);
    m_leftLayout->addStretch(1);//垂直方向不伸缩

}




void tcpServerGui::CreateRightWig()
{
    createRecvZone();
    createSendZone();
}
void tcpServerGui::createRecvZone()
{
    QVBoxLayout* recvZoneLayout=new QVBoxLayout(m_wigRightUp);
    recvZoneLayout->setMargin(0);
    m_tbRecvText=new QTextBrowser;
    recvZoneLayout->addWidget(m_tbRecvText);

}
void tcpServerGui::createSendZone()
{
    QVBoxLayout* sendZoneLayout=new QVBoxLayout(m_wigRightBottom);
    sendZoneLayout->setMargin(0);

    QHBoxLayout* rightMidLayout=new QHBoxLayout;
    QLabel *tip=new QLabel("客户端");
    rightMidLayout->addWidget(tip);

    m_cbAllConnects=new QComboBox;
    m_nClientCount=0;
    m_cbAllConnects->addItem(QString("allConnects(%1)").arg(m_nClientCount));
    rightMidLayout->addWidget(m_cbAllConnects);
    rightMidLayout->addSpacing(5);

    m_btnDisConnect=new QPushButton("断开");
    connect(m_btnDisConnect,&QPushButton::clicked,
            [this](){
            if(m_cbAllConnects->currentIndex()==0){
                QMessageBox::warning(this,"chooes fali","请选择一个客户端");
                return;
            }

            if(!m_tcpServerThread)return;
            QString ip=m_cbAllConnects->currentText().split(":")[0];
            quint16 port=m_cbAllConnects->currentText().split(":")[1].toInt();
            emit m_tcpServerThread->sig_closeOneClient(ip,port);

            m_cbAllConnects->removeItem(m_cbAllConnects->currentIndex());
            m_nClientCount--;
            m_cbAllConnects->setItemText(0,QString("allConnects(%1)").arg(m_nClientCount));


    });
    rightMidLayout->addWidget(m_btnDisConnect);
    rightMidLayout->addStretch(1);

    sendZoneLayout->addLayout(rightMidLayout);
    sendZoneLayout->setStretchFactor(rightMidLayout,0);


    QHBoxLayout* tempLayout=new QHBoxLayout;
    m_teSendText=new QTextEdit;
    m_btnSendMsg=new QPushButton("发送");
    connect(m_btnSendMsg,&QPushButton::clicked,
            this,&tcpServerGui::slot_sendMsg);
    m_btnSendMsg->setMaximumHeight(m_teSendText->width());
    tempLayout->addWidget(m_teSendText);
    tempLayout->addWidget(m_btnSendMsg);

    sendZoneLayout->addLayout(tempLayout);
    sendZoneLayout->setStretchFactor(tempLayout,1);
}

void tcpServerGui::slot_connectOrNot(bool checkAbled)
{
    if(checkAbled){
        m_tcpServerThread=new tcpServerThread(this);

        connect(m_tcpServerThread,&tcpServerThread::sig_connectSuccessMap,
                [this](QString ip,quint16 port){
                m_tbRecvText->append(QString("connect with client(%1:%2) success").arg(
                           ip).arg(port));

                m_nClientCount++;
                m_cbAllConnects->setItemText(0,QString("allConnects(%1)").arg(m_nClientCount));
                m_cbAllConnects->addItem(QString("%1:%2").arg(ip).arg(port));

        });


        connect(m_tcpServerThread,&tcpServerThread::sig_clientDisConnectMap,
                [this](QString ip,quint16 port){
                if(!m_tcpServerThread)return;
                m_tbRecvText->append(QString("client(%1:%2) disConnect").arg(
                           ip).arg(port));

                //server端监听状态由自己确定，不应该由客户端控制
//                m_btnConnect->setText("连接");
//                m_btnConnect->setChecked(false);
        });



        if(!m_tcpServerThread->start(m_cbLocalIp->currentText(),
                              m_leLocalPort->text().toInt()))
        {
            QMessageBox::warning(this,"connect error","tcpServer监听失败");
            m_btnConnect->setChecked(false);
            delete m_tcpServerThread;
            m_tcpServerThread=nullptr;
            return;
        }

        //read msg
        connect(m_tcpServerThread,&tcpServerThread::sig_recvMsgMap,
                [this](QByteArray buf,QString remoteIp,quint16 remotePort){
                QString time;
                if(m_cbShowTime->checkState()==Qt::CheckState::Checked)
                    time=QDateTime::currentDateTime().
                                toString("yyyy-mm-dd hh:mm:ss");
                else time="";

                QString strBuf=QString("%1 recvMsg from [%2:%3]:%4").arg(time).arg(
                            remoteIp).arg(remotePort).arg(QString(buf));

                if(m_cbCodeHex->checkState()==Qt::CheckState::Checked){
                    strBuf=QString("%1 recvMsg from [%2:%3]:%4").arg(time).arg(
                                remoteIp).arg(remotePort).arg(QString(buf.toHex(' ')));

                }
                QFont temp;
                temp.setPointSize(12);
                m_tbRecvText->setTextColor(QColor(224,0,0));
                m_tbRecvText->setCurrentFont(temp);
                m_tbRecvText->append(strBuf);

        });

        m_btnConnect->setText("断开");
        return;
    }//if

    if(!m_tcpServerThread)
        return;

    m_btnConnect->setText("连接");
    if(m_timer&&m_timer->isActive()){
        m_timer->stop();
        delete m_timer; m_timer=nullptr;
    }
    emit m_tcpServerThread->sig_serverDisconnect();

    //复原allConnect
    m_cbAllConnects->clear();
    m_nClientCount=0;
    m_cbAllConnects->addItem(QString("allConnects(%1)").arg(m_nClientCount));

}

void tcpServerGui::slot_timeOut(int state)
{
    if(state!=Qt::CheckState::Checked){
        if(!m_timer) return;
        m_timer->stop();
        delete m_timer; m_timer=nullptr;
        return;
    }

    m_timer=new QTimer(this);
    m_timer->setInterval(m_sbRepeatTimeInterval->value());
    connect(m_timer,&QTimer::timeout,
            this,&tcpServerGui::slot_sendMsg);
    m_timer->start();

}

void tcpServerGui::slot_sendMsg()
{
    if(!m_tcpServerThread || m_teSendText->toPlainText().isEmpty())
    {
        QMessageBox::warning(this,"warning","连接没打开或没有输入发送内容");
        m_cbRepeat->setChecked(false);//避免定时器不断触发,无法操作
        return;
    }

    //判断当前是否有连接
    if(m_nClientCount<=0){
        QMessageBox::warning(this,"warning","当前没有可用的连接");
        m_cbRepeat->setChecked(false);//避免定时器不断触发,无法操作
        return ;
    }

    QString resMsg=m_teSendText->toPlainText();

    //hex string need to change string
    if(m_cbSCodeHex->checkState()==Qt::CheckState::Checked){
        QByteArray tempBuf=resMsg.toLatin1();
        resMsg=QString(tempBuf.fromHex(tempBuf));
    }

    //saving msg need to send
    QByteArray buf;
    buf.append(resMsg);
    if(m_cbSCodeHex->checkState()==Qt::CheckState::Checked)
         buf=buf.toHex();


    // setting recv zone
    if(m_cbShowSendMsg->checkState()==Qt::CheckState::Checked)
    {
        QFont font;
        font.setPointSize(10);
        m_tbRecvText->setTextColor(QColor(0,112,224));
        m_tbRecvText->setCurrentFont(font);
        if(m_cbCodeHex->checkState()==Qt::CheckState::Checked){
            resMsg=QString(resMsg.toLatin1().toHex(' '));
        }
        QString time;
        if(m_cbShowTime->checkState()==Qt::CheckState::Checked)
            time=QDateTime::currentDateTime().
                            toString("yyyy-mm-dd hh:mm:ss");
        else time="";
        QString msg=QString("%1 sendMsg:%2").arg(time).arg(resMsg);
        m_tbRecvText->append(msg);
    }

    //广播方式
    if(m_cbAllConnects->currentIndex()==0)
       emit m_tcpServerThread->sig_sendMsgToAllClientMap(buf,buf.size());

    //单播方式
    else{
        QString ip=m_cbAllConnects->currentText().split(":")[0];
        quint16 port=m_cbAllConnects->currentText().split(":")[1].toInt();
        emit m_tcpServerThread->sig_sendMsgMap(ip,port,buf,buf.size());
    }




}





