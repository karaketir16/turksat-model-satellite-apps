#include <QCoreApplication>
#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QElapsedTimer>
#include <QTimer>

//#include <QProcess>

#include <QDataStream>

#include <QFile>

#include <QSerialPort>

#include <../protocol/mainobj.h>

QByteArray getted;
QString serialPortName = "ttyUSB1";
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QSerialPort serial;
//    auto serialPortName = "ttyUSB1";
//    serial.setPortName(serialPortName);
//    serial.setBaudRate(QSerialPort::Baud115200);
//    qDebug() << serial.open(QSerialPort::ReadWrite);

    QElapsedTimer tim;

    uint64_t totalCounter = 0, wrong  = 0, correct = 0;

    mainObj mmm;
    bool once = true;
    QObject::connect(&mmm, &mainObj::receiveRX, [&](Receive_Package_16 &rx){
        if(once)
        {
            tim.start();
            once = false;
        }

        uint8_t len = rx.lenght_low;

        len += 4;

        uint8_t sum = 0;
        QByteArray bytes(len, 0x00);
        memcpy(bytes.data(), &rx, len);
        for(int i = 3; i < len; i++){
            sum += bytes.data()[i];
        }


        bytes.remove(0,8);
        bytes.chop(1);

        totalCounter++;
        if(sum == 0xff)
        {
            qDebug() << "size: "<<bytes.size();
            correct++;
            if(bytes.size() == 77)
            {
                qDebug("Write");
                QFile file("./gettedData2.txt");
                if( ! file.open(QIODevice::ReadWrite | QIODevice::Truncate)){
                    qDebug() << "Cannot Open";
                }
                else{

                    QDataStream out(&file);
                    qDebug() << "written: " << out.writeRawData(getted.data(), getted.size());

                    file.close();
                }
            }
            else
                getted.push_back(bytes);
        }
        else wrong++;
        qDebug() << "total: " << totalCounter << " correct: " << correct << " wrong: " << wrong;
        qDebug() << "Now: "<< getted.size() << " goal: "<< (1 << 20) << " %" << (getted.size()*100) / (1 << 20) <<  " time: " << tim.elapsed() / 1000;
    });
//    QObject::connect(&mmm, &mainObj::receiveTXStat, test, &SerialThread::receiveTXStatus);
//    QObject::connect(test, &SerialThread::send, &mmm, &mainObj::send);

//    QByteArray buffer;

////    bool once = true;
//    while(1){
//        buffer.push_back(serial.readAll());
//        while( buffer.size() > 0 && 0x7e != buffer.data()[0]){
//            wrong++;
//            qDebug() << "removed "<< buffer.data()[0];
//            buffer.remove(0,1);
//        }
//        if(buffer.size() == 0 ) continue;


//    }



//    std::function<void(void)> func;
//    func = [&](){
//        if(once)
//        {
//            tim.start();
//            once = false;
//        }
//        qDebug() <<"get";

//        while(buffer.size() > 0){
//            buffer.push_back(serial.readAll());
//            while( buffer.size() > 0 && 0x7e != buffer.data()[0]){
//                wrong++;
//                qDebug() << "removed "<< buffer.data()[0];
//                buffer.remove(0,1);
//            }
//            if(buffer.size() == 0 ) return;

//            QDataStream ds (buffer.mid(1,2));

//            ds.setByteOrder(QDataStream::BigEndian);
//            uint16_t len;

//            ds >> len;

//    //        qDebug() << len;

//            len += 4;

//            auto lenNow = (int)buffer.size();
//            if(len > lenNow)
//            {
//    //            qDebug() << "len: " << len << " lenNow: " << lenNow;
//    //            qDebug() << buffer.toHex();
//                return;
//            }
//            auto bytes = buffer.left(len);
//            buffer.remove(0,len);

//            uint8_t sum = 0;

//            for(int i = 3; i < len; i++){
//                sum += bytes.data()[i];
//            }

//    //        qDebug() << bytes.toHex() << " " << bytes.size();

//            bytes.remove(0,8);
//            bytes.chop(1);

//            totalCounter++;
//            if(sum == 0xff)
//            {
//                qDebug() << "size: "<<bytes.size();
//                correct++;
//                if(bytes.size() == 77)
//                {
//                    qDebug("Write");
//                    QFile file("./gettedData2.txt");
//                    if( ! file.open(QIODevice::ReadWrite | QIODevice::Truncate)){
//                        qDebug() << "Cannot Open";
//                    }
//                    else{

//                        QDataStream out(&file);
//                        qDebug() << "written: " << out.writeRawData(getted.data(), getted.size());

//                        file.close();
//                    }
//                }
//                else
//                    getted.push_back(bytes);
//            }

//            else wrong++;
//            qDebug() << "total: " << totalCounter << " correct: " << correct << " wrong: " << wrong;
//            qDebug() << "Now: "<< getted.size() << " goal: "<< (1 << 20) << " %" << (getted.size()*100) / (1 << 20) <<  " time: " << tim.elapsed() / 1000;
//        }
//};
//    QObject::connect(&serial, &QSerialPort::readyRead, func);

    return a.exec();
}
