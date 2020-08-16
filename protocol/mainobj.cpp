#include "mainobj.h"

extern QString serialPortName;

mainObj::mainObj(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort();
    serial->setPortName(serialPortName);
    serial->setBaudRate(QSerialPort::Baud115200);
    while( ! serial->open(QSerialPort::ReadWrite)){
        qDebug() << "Cannot open " << serialPortName;
    }
    qDebug() << "opened UART";

    connect(serial, &QSerialPort::readyRead, this, &mainObj::receiveData);
}


void mainObj::receiveData(){
    qDebug() << "readyRead";
ReRead:
    qDebug() << "read??";
//    while (serial->bytesAvailable()) {


//    }
    QByteArray income = serial->readAll();
    incomeBuffer.append(income);
    qDebug() << incomeBuffer.toHex();

    while(incomeBuffer.size() > 0 && incomeBuffer.data()[0] != START_DELIMITER){
        incomeBuffer.remove(0,1);
    }
    int bufferSize = incomeBuffer.size();
    bool cont = true;
    while( (bufferSize = incomeBuffer.size()) > (int) sizeof (Header) && cont){
        Header * header = ( Header *) incomeBuffer.data();
        switch (header->frameType) {
            case TX_STATUS_RESPONSE:
                if(bufferSize >= (int) sizeof (Transmit_Status)){
                    Transmit_Status * ts = (Transmit_Status *) incomeBuffer.data();
                    emit receiveTXStat(*ts);
                    incomeBuffer.remove(0,sizeof (Transmit_Status));
                }
                else{
                    cont = false;
                }
                break;
            case RX_RESPONSE_16:
                Receive_Package_16 * rx = (Receive_Package_16 *) incomeBuffer.data();
                auto packageLenght = rx->lenght_low + 4;
                if(bufferSize >= (int) packageLenght){
                    emit receiveRX(*rx);
                    incomeBuffer.remove(0,packageLenght);
                }else{
                    cont = false;
                }
                break;
        }
    }
//    if(serial->bytesAvailable()){
//        goto ReRead;
//    }
}

void mainObj::send(QByteArray &toSent){
    qDebug() << "inside send, to send: " << toSent.toHex();
    const qint64 bytesWritten = serial->write(toSent);
    if (bytesWritten == -1) {
        qDebug() << QObject::tr("Failed to write the data to port %1, error: %2")
                          .arg(serialPortName).arg(serial->errorString()) << endl;
//        return false;
    } else if (bytesWritten != toSent.size()) {
        qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2")
                          .arg(serialPortName).arg(serial->errorString()) << endl;
//        return false;
    } else if (!serial->waitForBytesWritten(1000)) {
        qDebug() << QObject::tr("Operation timed out or an error "
                                      "occurred for port %1, error: %2")
                          .arg(serialPortName).arg(serial->errorString()) << endl;
//        return false;
    }
        qDebug() << "Sent to uart Succeess";
//    return true;
}
