#include "arduinonanoobject.h"
#include <QDebug>
#include <QElapsedTimer>

extern QString serialPortNameNano;
ArduinoNanoObject::ArduinoNanoObject(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort();
    serial->setPortName(serialPortNameNano);
    serial->setBaudRate(QSerialPort::Baud115200);
    if( ! serial->open(QSerialPort::ReadWrite)){
        qDebug() << "Cannot open " << serialPortNameNano;
    }
    else
        qDebug() << "opened UART";

    connect(serial, &QSerialPort::readyRead, this, &ArduinoNanoObject::receiveData);
}

QElapsedTimer nanoTimeTest;

void ArduinoNanoObject::receiveData(){
    QByteArray income = serial->readAll();
    incomeBuffer.append(income);
//    qDebug() << "Receive Nano: " << incomeBuffer.toHex();

    while(incomeBuffer.size() > 0 && incomeBuffer.data()[0] != NANO_START_DELIMITER){
        auto rem = incomeBuffer.remove(0,1);
//        qDebug() << "removed nano: " << rem.toHex();
    }
    int bufferSize = incomeBuffer.size();
    bool cont = true;
    while( (bufferSize = incomeBuffer.size()) >= (int) sizeof (nano_package) && cont){
        nano_package * np = ( nano_package *) incomeBuffer.data();
        if(ckeckChecksum(np)){
//            qDebug() << "Nano Timer Test: " <<nanoTimeTest.restart();
            emit receive(*np);
            incomeBuffer.remove(0,sizeof (nano_package));
        }
        else
            incomeBuffer.remove(0,1);
    }
}
