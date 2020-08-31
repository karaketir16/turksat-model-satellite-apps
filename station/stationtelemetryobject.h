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
#include <queue>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QFile>

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



    void received_COMMAND_Altitude_Calibrate() override;
    void received_COMMAND_Seperate_Carrier() override;
    void received_COMMAND_Reset_Telemetry_Number() override;
    void received_COMMAND_Reset_Package_Number() override;
    void received_COMMAND_Set_ManuelThrust_off() override;
    void received_COMMAND_Set_ManuelThrust_on() override;

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

public slots:
    void videoReadyChecker();
    void videoSender(bool fileName);
    void loop();
    void sendSeperateCarrier();
signals:
    void newTelemetryData(Telemetry_Data);
};

#endif // STATIONTELEMETRYOBJECT_H
