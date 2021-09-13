#include <QCoreApplication>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QElapsedTimer>
#include <QTimer>
#include <QProcess>
#include <QDataStream>
#include <QFile>
#include <QSerialPort>
#include <../protocol/mainobj.h>


extern "C" {
#include "my_i2c/my_i2c.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
}

#include "satellitetelemetryobject.h"
#include "arduinonanoobject.h"
//#include "./lsm9ds0/SFE_LSM9DS0.h"

#include "./lsm9ds1/SparkFunLSM9DS1.h"

QByteArray getted;
QString serialPortName = "ttyS0";
QString serialPortNameNano = "ttyUSB0";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Hello Test";
    qRegisterMetaType<uint16_t>("uint16_t");

    //yakma
    QProcess::execute("gpio -g mode 5 out");
    QProcess::execute("gpio -g write 5 0");


    SatelliteTelemetryObject tmObj;

    QProcess::execute("gpio -g mode 18 out");
    //QProcess::execute("gpio -g write 18 0");

    QProcess::execute("gpio -g mode 17 out");
    QProcess::execute("gpio -g mode 17 down");

    QProcess::execute("gpio -g write 17 0");

    tmObj.beepBuzzer(50);
    usleep(1000 * 1000);


//    LSM9DS1 lsm1;
//    qDebug() << "new: " << lsm1.begin();

//    while(true){

//        qDebug() << lsm1.calcAccel(lsm1.readAccel(Z_AXIS));
//        qDebug() << lsm1.readAccel(Z_AXIS);
//        lsm1.readTemp();
//        qDebug() << lsm1.temperature;

//        usleep(1000 * 500);
//    }

//    return 0;

    ArduinoNanoObject ardn;

    QProcess::execute("gpio -g mode 12 pwm");
    QProcess::execute("gpio pwm-ms");
    QProcess::execute("gpio pwmc 192");
    QProcess::execute("gpio pwmr 2000");
    QProcess::execute("gpio -g pwm 12 " + QString::number(SEPERATOR_NOT_SEPERATED));

    usleep(1000 * 500);

    tmObj.beepBuzzer(50);
    usleep(1000 * 50);
    tmObj.beepBuzzer(50);

    usleep(1000 * 500);

    QProcess::execute("gpio -g mode 13 pwm");
    QProcess::execute("gpio pwm-ms");
    QProcess::execute("gpio pwmc 192");
    QProcess::execute("gpio pwmr 2000");

    usleep(1000 * 1000);

    QObject::connect(&ardn, &ArduinoNanoObject::receive, &tmObj, &SatelliteTelemetryObject::received_Nano_Package);

    tmObj.start();

    return a.exec();
}
