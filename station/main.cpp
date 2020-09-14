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
#include <QDateTime>

QString serialPortName = "ttyUSB0";


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<uint16_t>("uint16_t");

    StationTelemetryObject tmObj;
    tmObj.start();


    MainWindow w;
    w.stationTelemetryObject = &tmObj;
    w.show();

    QObject::connect(&tmObj, &StationTelemetryObject::newTelemetryData, &w, &MainWindow::newTelemetryData);
    QObject::connect(&w, &MainWindow::commandSend, &tmObj, &StationTelemetryObject::commandSend);
    QObject::connect(&w, &MainWindow::startCSVsave, &tmObj, &StationTelemetryObject::startCSVsave);
    return a.exec();
}
