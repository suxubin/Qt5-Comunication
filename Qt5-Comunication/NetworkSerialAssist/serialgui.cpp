#include "serialgui.h"
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
#include "serialthread.h"
//#include <QSerialPortInfo>
#include <QTimer>
#include <QDateTime>


//Q_DECLARE_METATYPE(QString);
Q_DECLARE_METATYPE(QByteArray);
SerialGui::SerialGui(QWidget *parent)
    : QWidget(parent),m_serialThread(nullptr),m_timer(nullptr)
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

SerialGui::~SerialGui()
{
    //直接关闭后，找不到接收信号的对象，报错,将父类设成this

//    if(m_serialThread){
//        emit m_serialThread->sig_close();
//    }

}

void SerialGui::createLeftWig()
{
    m_leftLayout=new QVBoxLayout(m_wigLeft);
    m_leftLayout->setMargin(0);//设置不需要留边框
    createSerialSetting();
    createRecvSetting();
    createSendSetting();

}
void SerialGui::createSerialSetting()
{
    QGroupBox* serialGB=new QGroupBox;
    serialGB->setFlat(true);
    serialGB->setTitle("串口设置");
    QGridLayout* serialLayout=new QGridLayout;

    int lines=0;

    QLabel* port=new QLabel("串口号");
    serialLayout->addWidget(port,lines,0);
    m_cbPortName=new QComboBox;
    for(int i=1;i<100;i++){
        QString res=QString("COM%1").arg(i);
        m_cbPortName->addItem(res);
    }
    //get host avaluiable serial

    serialLayout->addWidget(m_cbPortName,lines,1);


    QLabel* buadRate=new QLabel("波特率");
    serialLayout->addWidget(buadRate,++lines,0);
    m_cbBuadRate=new QComboBox;
    QMetaEnum baudRate=QMetaEnum::fromType<QSerialPort::BaudRate>();
    for (int i=0;i<baudRate.keyCount();i++) {
        m_cbBuadRate->addItem(QString("%1").arg(baudRate.value(i)));
        //m_cbBuadRate->addItem(baudRate.valueToKey(baudRate.value(i)));
    }
    m_cbBuadRate->setCurrentIndex(3);
    serialLayout->addWidget(m_cbBuadRate,lines,1);

    QLabel* dataBit=new QLabel("数据位");
    serialLayout->addWidget(dataBit,++lines,0);
    m_cbDataBit=new QComboBox;
    QMetaEnum DataBit=QMetaEnum::fromType<QSerialPort::DataBits>();
    for (int i=0;i<DataBit.keyCount();i++) {
        m_cbDataBit->addItem(QString("%1").arg(DataBit.value(i)));
        //m_cbDataBit->addItem(DataBit.valueToKey(DataBit.value(i)));
    }
    m_cbDataBit->setCurrentIndex(3);
    serialLayout->addWidget(m_cbDataBit,lines,1);

    QLabel* stopBit=new QLabel("停止位");
    serialLayout->addWidget(stopBit,++lines,0);
    m_cbStopBit=new QComboBox;
    QMetaEnum StopBit=QMetaEnum::fromType<QSerialPort::StopBits>();
    for (int i=0;i<StopBit.keyCount();i++) {
        m_cbStopBit->addItem(QString("%1").arg(StopBit.value(i)));
        //m_cbStopBit->addItem(StopBit.valueToKey(StopBit.value(i)));
    }
    serialLayout->addWidget(m_cbStopBit,lines,1);

    QLabel* checkBit=new QLabel("校验位");
    serialLayout->addWidget(checkBit,++lines,0);
    m_cbParity=new QComboBox;
    QMetaEnum Parity=QMetaEnum::fromType<QSerialPort::Parity>();
    for (int i=0;i<Parity.keyCount();i++) {
        m_cbParity->addItem(QString("%1").arg(Parity.value(i)));
        //m_cbParity->addItem(Parity.valueToKey(Parity.value(i)));
    }
    serialLayout->addWidget(m_cbParity,lines,1);

    QLabel* flowControl=new QLabel("流控");
    serialLayout->addWidget(flowControl,++lines,0);
    m_cbFlowControl=new QComboBox;
    QMetaEnum FlowControl=QMetaEnum::fromType<QSerialPort::FlowControl>();
    for (int i=0;i<FlowControl.keyCount();i++) {
        m_cbFlowControl->addItem(QString("%1").arg(FlowControl.value(i)));
        //m_cbFlowControl->addItem(FlowControl.valueToKey(FlowControl.value(i)));
    }
    serialLayout->addWidget(m_cbFlowControl,lines,1);

    QLabel* tip=new QLabel("连接/断开");
    m_btnConnect=new QPushButton("连接");
    m_btnConnect->setCheckable(true);
    connect(m_btnConnect,&QPushButton::toggled,
            this,&SerialGui::slot_connectOrNot);
    serialLayout->addWidget(tip,++lines,0);
    serialLayout->addWidget(m_btnConnect,lines,1);

    serialGB->setLayout(serialLayout);
    m_leftLayout->addWidget(serialGB);

}
void SerialGui::createRecvSetting()
{
    QGroupBox* recvGB=new QGroupBox;
    recvGB->setTitle("接收设置");
    recvGB->setFlat(true);

    QVBoxLayout* recvLayout=new QVBoxLayout;

    m_cbCodeHex=new QCheckBox("十六进制接收");
    m_cbAutoChangeLine= new QCheckBox("自动换行");
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
    recvLayout->addWidget(m_cbAutoChangeLine);
    recvLayout->addWidget(m_cbShowSendMsg);
    recvLayout->addWidget(m_cbShowTime);
    recvLayout->addWidget(m_btnClearRecvZone);

    recvGB->setLayout(recvLayout);
    m_leftLayout->addWidget(recvGB);

}
void SerialGui::createSendSetting()
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
            this,&SerialGui::slot_timeOut);

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

void SerialGui::CreateRightWig()
{
    createRecvZone();
    createSendZone();
}
void SerialGui::createRecvZone()
{
    QVBoxLayout* recvZoneLayout=new QVBoxLayout(m_wigRightUp);
    recvZoneLayout->setMargin(0);
    m_tbRecvText=new QTextBrowser;
    recvZoneLayout->addWidget(m_tbRecvText);

}
void SerialGui::createSendZone()
{
    QVBoxLayout* sendZoneLayout=new QVBoxLayout(m_wigRightBottom);
    sendZoneLayout->setMargin(0);

    QHBoxLayout* tempLayout=new QHBoxLayout;
    m_teSendText=new QTextEdit;
    m_btnSendMsg=new QPushButton("发送");
    connect(m_btnSendMsg,&QPushButton::clicked,
            this,&SerialGui::slot_sendMsg);
    m_btnSendMsg->setMaximumHeight(m_teSendText->width());

    tempLayout->addWidget(m_teSendText);
    tempLayout->addWidget(m_btnSendMsg);
    sendZoneLayout->addLayout(tempLayout);

}


void SerialGui::slot_connectOrNot(bool checkAbled)
{
    if(checkAbled){
        m_serialThread=new serialThread(this);
        bool res=m_serialThread->start(m_cbPortName->currentText(),m_cbBuadRate->currentIndex(),
                            m_cbDataBit->currentIndex(),m_cbStopBit->currentIndex(),
                            m_cbParity->currentIndex(),m_cbFlowControl->currentIndex());

        if(!res){
            QMessageBox::warning(this,"serial error","串口打开失败");
            m_btnConnect->setChecked(false);
            delete m_serialThread;
            m_serialThread=nullptr;
            return;
        }

        //read msg
        qRegisterMetaType<QByteArray>();
        connect(m_serialThread,&serialThread::sig_recvMsgMap,
                [this](QByteArray buf){
                    QString time;
                    if(m_cbShowTime->checkState()==Qt::CheckState::Checked)
                        time=QDateTime::currentDateTime().
                                        toString("yyyy-mm-dd hh:mm:ss");
                    else time="";

                    QString strBuf=QString("%1 recvMsg:%2").arg(time).arg(QString(buf));
                    if(m_cbCodeHex->checkState()==Qt::CheckState::Checked){
                        strBuf=QString("%1 recvMsg:%2").arg(time).arg(QString(buf.toHex(' ')));

                    }
                    QFont temp;
                    temp.setPointSize(12);
                    m_tbRecvText->setTextColor(QColor(224,0,0));
                    m_tbRecvText->setCurrentFont(temp);
                    m_tbRecvText->append(strBuf);
        });

        m_btnConnect->setText("断开");
        return;
    }

    if(!m_serialThread)
        return;

    m_btnConnect->setText("连接");
    if(m_timer&&m_timer->isActive()){
        m_timer->stop();
        delete m_timer; m_timer=nullptr;
    }
    emit m_serialThread->sig_close();

}

void SerialGui::slot_sendMsg()
{
    if(!m_serialThread || m_teSendText->toPlainText().isEmpty())
    {
        QMessageBox::warning(this,"warning","连接没打开或没有输入发送内容");
        m_cbRepeat->setChecked(false);
        return;
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

    emit m_serialThread->sig_sendMsg(buf);


}

void SerialGui::slot_timeOut(int state)
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
            this,&SerialGui::slot_sendMsg);
    m_timer->start();


}



















