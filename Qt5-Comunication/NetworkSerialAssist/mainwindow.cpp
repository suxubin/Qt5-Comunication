#include "mainwindow.h"
#include <QTabWidget>
#include "serialgui.h"
#include "udpgui.h"
#include "tcpclientgui.h"
#include "tcpservergui.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("网络串口调试助手");
    this->resize(this->width(),this->height());

    m_tabWig=new QTabWidget;
    setCentralWidget(m_tabWig);

    m_serialGui=new SerialGui;
    m_tabWig->addTab(m_serialGui,"串口调试");

    m_udpGui=new udpGui;
    m_tabWig->addTab(m_udpGui,"udp调试");

    m_tcpClientGui=new tcpClientGui;
    m_tabWig->addTab(m_tcpClientGui,"tcp客户端");

    m_tcpServerGui=new tcpServerGui;
    m_tabWig->addTab(m_tcpServerGui,"tcp服务端");


}

MainWindow::~MainWindow()
{
    //和初始化生成方向相反
    if(m_tcpServerGui){
        delete m_tcpServerGui;
        m_tcpServerGui=nullptr;
    }

    if(m_tcpClientGui){
        delete m_tcpClientGui;
        m_tcpClientGui=nullptr;
    }

    if(m_udpGui){
        delete m_udpGui;
        m_udpGui=nullptr;
    }

    if(m_serialGui){
        delete m_serialGui;
        m_serialGui=nullptr;

    }


    if(m_tabWig){
        delete  m_tabWig;
        m_tabWig=nullptr;
    }

}

