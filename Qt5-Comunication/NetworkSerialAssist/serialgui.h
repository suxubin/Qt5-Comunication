#ifndef SERIALGUI_H
#define SERIALGUI_H

#include <QObject>
#include <QWidget>

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QLineEdit;
class QTextEdit;
class QTextBrowser;
class QCheckBox;
//class QRadioButton;
class QSpinBox;
class QPushButton;
class QTimer;

class serialThread;
class SerialGui : public QWidget
{
    Q_OBJECT
public:
    explicit SerialGui(QWidget *parent = nullptr);
    ~SerialGui();

    void createLeftWig();
    void CreateRightWig();

    void createSerialSetting();
    void createRecvSetting();
    void createSendSetting();

    void createRecvZone();
    void createSendZone();



signals:

private slots:
    void slot_connectOrNot(bool);
    void slot_sendMsg();

    void slot_timeOut(int);

public slots:




private:
    QWidget* m_wigLeft;
    QWidget* m_wigRightUp;
    QWidget* m_wigRightBottom;
    QVBoxLayout* m_leftLayout;


    //---------serial setting------------
    QComboBox* m_cbPortName;
    QComboBox* m_cbBuadRate;
    QComboBox* m_cbDataBit;
    QComboBox* m_cbStopBit;
    QComboBox* m_cbParity;
    QComboBox* m_cbFlowControl;
    QPushButton* m_btnConnect;

    //---------recv setting----------
    QCheckBox* m_cbCodeHex;
    QCheckBox* m_cbAutoChangeLine;
    QCheckBox* m_cbShowSendMsg;
    QCheckBox* m_cbShowTime;
    QPushButton* m_btnClearRecvZone;

    //------------send setting--------
    QCheckBox* m_cbSCodeHex;
    QCheckBox* m_cbRepeat;
    QSpinBox* m_sbRepeatTimeInterval;
    QPushButton* m_btnClearSendZone;

    //----------recv zone------
    QTextBrowser* m_tbRecvText;

    //-----------send zone-------
    QTextEdit* m_teSendText;
    QPushButton* m_btnSendMsg;


    serialThread* m_serialThread;

    QTimer* m_timer;


};

#endif // SERIALGUI_H
