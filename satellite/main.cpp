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




extern "C" {
#include "bme280/bme280.h"
#include "bme280/bme280_wrapper.h"
#include "my_i2c/my_i2c.h"

#include <sys/ioctl.h>
#include <linux/ioctl.h>

#include <stdlib.h>
#include <unistd.h>
}

#include "satellitetelemetryobject.h"

void loop();
void setup();

#define pinCtl_BTN "0"

QByteArray getted;
QString serialPortName = "ttyUSB0";
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    qDebug() << "Little";
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
qDebug() << "Big";
#endif
//    uint8_t asd = 0;
//    int file = I2C_init("/dev/i2c-0");
//    qDebug() << "asd: "<< i2c_read_buff(file,0xd6,0x0f,&asd,1);
//    qDebug() << asd;
//    setup();
//    while(1) loop();
    qDebug() << sizeof (float);


    mainObj obj;
    SatelliteTelemetryObject tmObj;
    QObject::connect(&tmObj, &SatelliteTelemetryObject::send, &obj, &mainObj::send);
    QObject::connect(&obj, &mainObj::receive, &tmObj, &SatelliteTelemetryObject::received_DATA);
    tmObj.start();

//    int fff = 0;
//    fff = open("/sys/class/gpio/export", O_WRONLY);
//    write(fff, pinCtl_BTN, sizeof pinCtl_BTN);
//    close(fff);

//    // Configure as output
//    fff = open("/sys/class/gpio/gpio" pinCtl_BTN "/direction", O_WRONLY);
//    write(fff, "out", 3);
//    close(fff);

//    int pin = open("/sys/class/gpio/gpio" pinCtl_BTN "/value", O_WRONLY | O_SYNC);

//    write(pin, "1", 1);
//    usleep(1000 * 150);
//    write(pin, "0", 1);
//    usleep(1000 * 150);
//    write(pin, "1", 1);
//    close(pin);

//    fff = open("/sys/class/gpio/unexport", O_WRONLY);
//    write(fff, pinCtl_BTN, sizeof pinCtl_BTN);
//    close(fff);
    qDebug() << "end";
//    bme_File_and_Addr bme_f_a;
//    bme_f_a.file = I2C_init("/dev/i2c-0");
//    bme_f_a.addr = 0x76 * 2;

//    bme280_dev bme280;
//    bme280.intf = BME280_I2C_INTF;
//    bme280.read = user_i2c_read;
//    bme280.write = user_i2c_write;
//    bme280.delay_us = user_delay_us;
//    bme280.intf_ptr = &bme_f_a;

//    qDebug() << "result: " << bme280_init(&bme280);



//    QElapsedTimer tim;

//    uint64_t totalCounter = 0, wrong  = 0, correct = 0;

//    mainObj mmm;
//    bool once = true;
//    QFile file("../getted.avi");
//    if( ! file.open(QIODevice::ReadWrite | QIODevice::Truncate)){
//        qDebug() << "Cannot Open";
//    }
//    QDataStream out(&file);
//    QObject::connect(&mmm, &mainObj::receiveRX, [&](Receive_Package_16 &rx){
//        if(once)
//        {
//            tim.start();
//            once = false;
//        }
//        totalCounter++;
//        bool check;
//        uint8_t RSSI;
//        auto bytes = get_data_Receive_Package_16(rx,check, &RSSI);
//        if(check)
//        {
//            qDebug() << "size: "<<bytes.size();
//            correct++;
//            file.write(bytes);
//            file.flush();
//        }
//        else
//            wrong++;
//        qDebug() << "total: " << totalCounter << " correct: " << correct << " wrong: " << wrong <<" RSSI: " << RSSI;
//        qDebug() << "Now: "<< file.size() << " goal: "<< (1 << 20) << " %" << (file.size()*100) / (1 << 20) <<  " time: " << tim.elapsed() / 1000;
//    });
    return a.exec();
}
