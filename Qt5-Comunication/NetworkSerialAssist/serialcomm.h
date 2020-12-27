#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QSerialPort>

class serialComm : public QSerialPort
{
    Q_OBJECT
public:
    explicit serialComm(QObject *parent = nullptr);
    ~serialComm(){}


    bool start(QString _portName,int _baudRateInx,int _dataBitInx,
               int _stopBitInx,int _parityInx,int _flowControlInx);



signals:
    void sig_closeFinish();
    void sig_recvMsg(QByteArray);

public slots:
    void slot_close();
    void slot_sendMsg(QByteArray);


private:
    //QByteArray m_byteArr;




};



#endif // SERIALCOMM_H
