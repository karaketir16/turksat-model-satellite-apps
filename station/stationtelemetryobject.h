#ifndef STATIONTELEMETRYOBJECT_H
#define STATIONTELEMETRYOBJECT_H

#include "../protocol/abstracttelemetryobject.h"
#include <queue>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QSet>

#include "../protocol/abstracttelemetryobject.h"
#include "../protocol/telemetry_protocol.h"
#include "../protocol/mainobj.h"
#include <queue>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include "mainwindow.h"

class MainWindow;

class StationTelemetryObject : public AbstractTelemetryObject
{
    Q_OBJECT
    void run() override;
public:
    StationTelemetryObject();
    void received_PACKAGE_Telemetry_Data(Telemetry_Data data) override;
//    void received_PACKAGE_ACK(ACK ack) override;
    void received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name) override;
    void received_PACKAGE_Video_Data(Video_Data video_data) override;
    void received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK) override;



//    void received_COMMAND_Altitude_Calibrate(uint8_t) override;
//    void received_COMMAND_Seperate_Carrier(uint8_t) override;
//    void received_COMMAND_Reset_Telemetry_Number(uint8_t) override;
//    void received_COMMAND_Reset_Package_Number(uint8_t) override;
//    void received_COMMAND_Reset_Satellite_Status(uint8_t) override;
//    void received_COMMAND_Set_ManuelThrust_off(uint8_t) override;
//    void received_COMMAND_Set_ManuelThrust_on(uint8_t) override;
//    void received_COMMAND_Set_Thrust(uint8_t) override;
//    void received_COMMAND_Set_Seperator(uint8_t) override;
//    void received_COMMAND_Test_Thrust(uint8_t) override;

    void received_COMMAND(Command) override;

    uint32_t generateTelemetryNumber() override;


public:

    QString videoPath = "";
    bool videoReady = false;


    QByteArray videoData;
    uint64_t videoSize = 0;
    uint32_t videoPartCount = -1; // for videoSender function
    QSet<uint32_t> ackedVideoParts;

    uint32_t partPointer = 0;
    uint32_t videoSetTelemNumber = 0;
    uint8_t videoNameSetted = false;
    QVector<QByteArray> videoPackets;
    uint32_t lastPoint = 0;


    uint32_t videoSenderIndex = 0;

    uint32_t lastPointVideo = 0;

    mainObj * xBee;

    MainWindow * wind;


    void writeSaveValues() override;

public slots:
    void videoReadyChecker();
    void videoSender(bool fileName);
    void loop();
//    void sendSeperateCarrier();
//    void sendSetSeperator(uint8_t);
//    void sendSetEngineThrust(uint8_t);
//    void sendTestThrust(uint8_t);
//    void sendGroundSet(uint8_t);
//    void sendResetStatus(uint8_t);

    void commandSend(uint8_t, uint8_t);
signals:
    void newTelemetryData(Telemetry_Data);
};

#endif // STATIONTELEMETRYOBJECT_H
