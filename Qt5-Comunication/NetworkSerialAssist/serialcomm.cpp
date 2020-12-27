#include "serialcomm.h"
#include <QMetaEnum>

serialComm::serialComm(QObject *parent) : QSerialPort(parent)
{


}


bool serialComm::start(QString _portName, int _baudRateInx, int _dataBitInx,
                       int _stopBitInx, int _parityInx, int _flowControlInx)
{
    setPortName(_portName);

    if(!open(ReadWrite))
        return false;

    QMetaEnum __baudRate=QMetaEnum::fromType<QSerialPort::BaudRate>();
    if(!setBaudRate(__baudRate.value(_baudRateInx)))
        return false;

    QMetaEnum __dataBit=QMetaEnum::fromType<QSerialPort::DataBits>();
    if(!setDataBits((DataBits)__dataBit.value(_dataBitInx)))
        return false;

    QMetaEnum __stopBit=QMetaEnum::fromType<QSerialPort::StopBits>();
    if(!setStopBits((StopBits)__stopBit.value(_stopBitInx)))
        return false;

    QMetaEnum __parity=QMetaEnum::fromType<QSerialPort::Parity>();
    if(!setParity((Parity)__parity.value(_parityInx)))
        return false;

    QMetaEnum __flowControl=QMetaEnum::fromType<QSerialPort::FlowControl>();
    if(!setFlowControl((FlowControl)__flowControl.value(_flowControlInx)))
        return false;

    connect(this,&QSerialPort::readyRead,
            [this](){
            QByteArray buf=readAll();
            emit sig_recvMsg(buf);

    });


    return true;

}

void serialComm::slot_close()
{
    close();
    emit sig_closeFinish();
}

void serialComm::slot_sendMsg(QByteArray buf)
{
    writeData(buf.data(),buf.size());

}













