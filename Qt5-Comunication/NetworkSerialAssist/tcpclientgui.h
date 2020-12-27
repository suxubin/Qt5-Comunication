#ifndef TCPCLIENTGUI_H
#define TCPCLIENTGUI_H

#include <QWidget>

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QLineEdit;
class QTextEdit;
class QTextBrowser;
class QCheckBox;
class QSpinBox;
class QPushButton;
class QTimer;
class tcpClientThread;

class tcpClientGui : public QWidget
{
    Q_OBJECT
public:
    explicit tcpClientGui(QWidget *parent = nullptr);

    void createLeftWig();
    void CreateRightWig();

    void createTcpSetting();
    void createRecvSetting();
    void createSendSetting();

    void createRecvZone();
    void createSendZone();

signals:

private slots:
    void slot_connectOrNot(bool);
    void slot_timeOut(int);
    void slot_sendMsg();

private:
    QWidget* m_wigLeft;
    QWidget* m_wigRightUp;
    QWidget* m_wigRightBottom;
    QVBoxLayout* m_leftLayout;

    //tcp client setting
    QComboBox* m_cbLocalIp;
    QLineEdit* m_leLocalPort; //是远端，要动很多地方不想改
    QPushButton* m_btnConnect;

    //recv setting
    QCheckBox* m_cbCodeHex;
    QCheckBox* m_cbShowSendMsg;
    QCheckBox* m_cbShowTime;
    QPushButton* m_btnClearRecvZone;

    //send setting
    QCheckBox* m_cbSCodeHex;
    QCheckBox* m_cbRepeat;
    QSpinBox* m_sbRepeatTimeInterval;
    QPushButton* m_btnClearSendZone;

    //----------recv zone------
    QTextBrowser* m_tbRecvText;

    //-----------send zone-------
//    QLineEdit* m_leRemoteIp;
//    QLineEdit* m_leRemotePort;
    QTextEdit* m_teSendText;
    QPushButton* m_btnSendMsg;


    tcpClientThread* m_tcpClientThread;
    QTimer* m_timer;



};

#endif // TCPCLIENTGUI_H
