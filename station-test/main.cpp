#include <QCoreApplication>
#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QElapsedTimer>
#include <QTimer>

//#include <QProcess>

#include <QDataStream>
#include <QFile>


#include "../protocol/protocol.h"

#include <../protocol/mainobj.h>

QString serialPortName = "ttyUSB1";

void checkSum(QByteArray& arr){
    uint8_t res = 0;
    for(int i =3;i<arr.size(); i++)
    {
        res += arr.data()[i];
    }
    arr.push_back(0xFF - res);
}

void packageIT(QByteArray &arr)
{
    QByteArray it;
    it.resize(8);

    it.data()[0] = 0x7E;
    it.data()[1] = 0x00;
    it.data()[2] = arr.size() + 5;
    it.data()[3] = 0x01; //frame type
    it.data()[4] = 0x01; //frame ID
    it.data()[5] = 0x00;
    it.data()[6] = 0x3C;
    it.data()[7] = 0x00;
    it.push_back(arr);
    checkSum(it);
    arr = it;
}

uint8_t ID_Counter;
uint8_t ID_Array[256];

//#include <../protocol/SerialThread.h>
#include "stationtelemetryobject.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


//    Transmit_Header asdasd;
//    asdasd.startDelimiter = 0x7E;
//    asdasd.lenght_low = 0x33;
//    asdasd.frameType = 0x89;
//    asdasd.frameID = 0x01;

//    QByteArray ba(sizeof (asdasd), 0x22);

    mainObj obj;
    StationTelemetryObject tmObj;
    QObject::connect(&tmObj, &StationTelemetryObject::send, &obj, &mainObj::send);
    QObject::connect(&obj, &mainObj::receive, &tmObj, &StationTelemetryObject::received_DATA);
    tmObj.start();



//    SerialThread * test = new SerialThread();
//    test->start();
////    test->start();
////    a.exec();
////    memcpy((void*)ba.data(), (void*)&asdasd, sizeof (asdasd));


////    QByteArray ba = QByteArray::fromRawData((char*)&asdasd, 5);
////    qDebug() << "startDelimiter: " << ((Transmit_Header*)ba.data())->startDelimiter;
////    qDebug() << "lenght: " << ((Transmit_Header*)ba.data())->lenght_low;
////    qDebug() << "frameType: " << ((Transmit_Header*)ba.data())->frameType;
////    qDebug() << "frameID: " << ((Transmit_Header*)ba.data())->frameID;
////    qDebug() <<"data: " << ba.toHex();
////    return 0;

//    QFile CurrentFile("../drop.avi");
//    if(!CurrentFile.open(QIODevice::ReadOnly)) return 0;
//    QByteArray DataFile = CurrentFile.readAll();

//    mainObj mmm;

//    QObject::connect(&mmm, &mainObj::receiveRX, test, &SerialThread::receiveRX);
//    QObject::connect(&mmm, &mainObj::receiveTXStat, test, &SerialThread::receiveTXStatus);
//    QObject::connect(test, &SerialThread::send, &mmm, &mainObj::send);

//    int i =0;
//    int xx = 1;
//    for(; i < DataFile.size(); i+=50)
//    {
//        auto sendThat = DataFile.mid(i, 50);
////        packageIT(sendThat);
//        Transmit_Request_16 tx = create_Transmit_Request_16(sendThat, 0x003C);
////        memcpy(&tx, sendThat.data(), sendThat.size());

//        test->transmitQueuesMutex.lock();
//        test->transmitQueues[1].enqueue(tx);
//        test->transmitQueuesMutex.unlock();

////        break;

////        serial.write(sendThat);
////        if( ! (--xx)){
////            break;
////        }
////        qDebug() << i + 50 << " of " << DataFile.size();
//    }
//    QByteArray dummy;
//    dummy.fill(0, 77);
//    packageIT(dummy);
//    Transmit_Request_16 tx;
//    memcpy(&tx, dummy.data(), dummy.size());
//    test->transmitQueuesMutex.lock();
//    test->transmitQueues[1].enqueue(tx);
//    test->transmitQueuesMutex.unlock();

//    qDebug() << "Done";

//    test->start();
    return a.exec();
}

