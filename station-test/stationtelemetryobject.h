#ifndef STATIONTELEMETRYOBJECT_H
#define STATIONTELEMETRYOBJECT_H

#include "../protocol/abstracttelemetryobject.h"
#include <queue>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QDebug>

class StationTelemetryObject : public AbstractTelemetryObject
{
    Q_OBJECT
    void run() override;
public:
    StationTelemetryObject();

    void received_PACKAGE_Telemetry_Data(Telemetry_Data data) override;
    void received_PACKAGE_ACK(ACK ack) override;
    void received_PACKAGE_Command (Command cmd) override;
    void received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name) override;
    void received_PACKAGE_Video_Data(Video_Data video_data) override;
    void received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK) override;

    void loop();

    void received_COMMAND_Altitude_Calibrate() override;
    void received_COMMAND_Seperate_Carrier() override;
    void received_COMMAND_Reset_Telemetry_Number() override;
    void received_COMMAND_Set_ManuelThrust_off() override;
    void received_COMMAND_Set_ManuelThrust_on() override;

    uint16_t receiver = 0x0037;
};

#endif // STATIONTELEMETRYOBJECT_H
