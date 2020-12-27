#ifndef UDPGUI_H
#define UDPGUI_H

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

class udpThread;
class udpGui : public QWidget
{
    Q_OBJECT
public:
    explicit udpGui(QWidget *parent = nullptr);

    void createLeftWig();
    void CreateRightWig();

    void createUdpSetting();
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

    //udp setting
    QComboBox* m_cbLocalIp;
    QLineEdit* m_leLocalPort;
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
    QLineEdit* m_leRemoteIp;
    QLineEdit* m_leRemotePort;
    QTextEdit* m_teSendText;
    QPushButton* m_btnSendMsg;


    udpThread* m_udpThread;
    QTimer* m_timer;

};

#endif // UDPGUI_H
