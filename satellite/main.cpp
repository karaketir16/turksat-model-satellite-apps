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
        totalCounter++;
        bool check;
        auto bytes = get_data_Receive_Package_16(rx,check);
        if(check)
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
        else
            wrong++;
        qDebug() << "total: " << totalCounter << " correct: " << correct << " wrong: " << wrong;
        qDebug() << "Now: "<< getted.size() << " goal: "<< (1 << 20) << " %" << (getted.size()*100) / (1 << 20) <<  " time: " << tim.elapsed() / 1000;
    });
//    QObject::connect(&mmm, &mainObj::receiveTXStat, test, &SerialThread::receiveTXStatus);

    return a.exec();
}
