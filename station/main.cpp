#include "mainwindow.h"

#include <QApplication>




#include <QCoreApplication>
#include <QDebug>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QElapsedTimer>
#include <QTimer>

//#include <QProcess>

#include <QDataStream>
#include <QFile>

#include "stationtelemetryobject.h"


#include "../protocol/protocol.h"

#include <../protocol/mainobj.h>

QString serialPortName = "ttyUSB0";


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<uint16_t>("uint16_t");


    StationTelemetryObject tmObj;
//    tmObj.start();


    MainWindow w;
    w.stationTelemetryObject = &tmObj;
    w.show();
//    tmObj.wind = &w;

    QObject::connect(&tmObj, &StationTelemetryObject::newTelemetryData, &w, &MainWindow::newTelemetryData);
//    QObject::connect(&w, &MainWindow::sendVideo, &tmObj, &StationTelemetryObject::videoSender, Qt::QueuedConnection);

    QObject::connect(&w, &MainWindow::seperateCarrier, &tmObj, &StationTelemetryObject::sendSeperateCarrier);
    QObject::connect(&w, &MainWindow::setSeperator, &tmObj, &StationTelemetryObject::sendSetSeperator);
    QObject::connect(&w, &MainWindow::setEngineThrust, &tmObj, &StationTelemetryObject::sendSetEngineThrust);

    return a.exec();
}
