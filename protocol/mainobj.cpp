#include "mainobj.h"
#include "protocol.h"
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
//ReRead:
//    qDebug() << "read??";
//    while (serial->bytesAvailable()) {


//    }
    QByteArray income = serial->readAll();
    incomeBuffer.append(income);
//    qDebug() << incomeBuffer.toHex();

    while(incomeBuffer.size() > 0 && incomeBuffer.data()[0] != START_DELIMITER){
        auto rem = incomeBuffer.remove(0,1);
        qDebug() << "removed: " << rem.toHex();
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
                auto packageLenght = (((uint16_t)rx->lenght_high) << 8) + rx->lenght_low + 4;
                if(bufferSize >= (int) packageLenght){
                    bool stat;
                    uint8_t RSSI;
                    auto data = get_data_Receive_Package_16(*rx, stat, &RSSI);
                    if(stat){
                        qDebug() << "data: " << data.toHex();
                        emit receive(data);
                        incomeBuffer.remove(0,packageLenght);
                    }
                    else
                        incomeBuffer.remove(0,1);

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

void mainObj::send(QByteArray toSent, uint16_t receiver){
    qDebug() << "inside send, to send data: " << toSent.toHex();
    auto sentPair = create_Transmit_Request_16(toSent,receiver);
    qDebug() << "inside send, to send xbee: " << to_byte_array(&sentPair.first, sentPair.second).toHex();
    const qint64 bytesWritten = serial->write(to_byte_array(&sentPair.first, sentPair.second));
    serial->waitForBytesWritten(1000);
//    if (bytesWritten == -1) {
//        qDebug() << QObject::tr("Failed to write the data to port %1, error: %2")
//                          .arg(serialPortName).arg(serial->errorString()) << endl;
////        return false;
//    } else if (bytesWritten != toSent.size()) {
//        qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2")
//                          .arg(serialPortName).arg(serial->errorString()) << endl;
////        return false;
//    } else if (!serial->waitForBytesWritten(1000)) {
//        qDebug() << QObject::tr("Operation timed out or an error "
//                                      "occurred for port %1, error: %2")
//                          .arg(serialPortName).arg(serial->errorString()) << endl;
////        return false;
//    }
//        qDebug() << "Sent to uart Succeess";
//    return true;
}
