#ifndef SATELLITETELEMETRYOBJECT_H
#define SATELLITETELEMETRYOBJECT_H

#include "../protocol/abstracttelemetryobject.h"
#include "../protocol/telemetry_protocol.h"

#include <QProcess>

#include <queue>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QElapsedTimer>
#include "lsm9ds0/SFE_LSM9DS0.h"
#include "nano_packets.h"

class SatelliteTelemetryObject : public AbstractTelemetryObject
{
    Q_OBJECT
    void run() override;
public:
    SatelliteTelemetryObject();
    void received_PACKAGE_Telemetry_Data(Telemetry_Data data) override;
//    void received_PACKAGE_ACK(ACK ack) override;
    void received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name) override;
    void received_PACKAGE_Video_Data(Video_Data video_data) override;
    void received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK) override;

    void received_COMMAND_Altitude_Calibrate(uint8_t) override;
    void received_COMMAND_Seperate_Carrier(uint8_t) override;
    void received_COMMAND_Reset_Telemetry_Number(uint8_t) override;
    void received_COMMAND_Reset_Package_Number(uint8_t) override;
    void received_COMMAND_Set_ManuelThrust_off(uint8_t) override;
    void received_COMMAND_Set_ManuelThrust_on(uint8_t) override;
    void received_COMMAND_Set_Thrust(uint8_t) override;
    void received_COMMAND_Set_Seperator(uint8_t) override;

    QByteArray Telemetry_sendThis;
    Telemetry_Data Telemetry_update;

    QTimer oneSecond;

    float groundPressure;
    float groundAltitude;




    uint16_t team_no = 53402;

    bool manuelThrust = true; // SET false this

    QFile videoFile;
    uint32_t expectedVideoParts = 0;
    QVector<QByteArray> videoData;
    uint32_t videoWritePointer = 0;


    QElapsedTimer testTimer;


    QElapsedTimer IMUspeedTimer;

    tripleFloat IMUspeed = {0,0,0};

    uint8_t lsmStatus = 0;


    uint8_t telemetryRefresh = 0;
//    uint8_t tele;

    float pressure0 = 1001.84;

public slots:
    void loop();

    void telemetrySetter();

    void telemetrySender();

    void received_Nano_Package(nano_package np);
};

#endif // SATELLITETELEMETRYOBJECT_H
