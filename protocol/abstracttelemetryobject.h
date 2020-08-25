#ifndef ABSTRACTTELEMETRYOBJECT_H
#define ABSTRACTTELEMETRYOBJECT_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include "telemetry_protocol.h"

class AbstractTelemetryObject : public QThread
{
    Q_OBJECT

public:
    explicit AbstractTelemetryObject(QObject *parent = nullptr);

    virtual void received_PACKAGE_Telemetry_Data(Telemetry_Data data) = 0;
    virtual void received_PACKAGE_ACK(ACK ack) = 0;
    virtual void received_PACKAGE_Command (Command cmd) = 0;
    virtual void received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name) = 0;
    virtual void received_PACKAGE_Video_Data(Video_Data video_data) = 0;
    virtual void received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK) = 0;


    virtual void received_COMMAND_Altitude_Calibrate() = 0;
    virtual void received_COMMAND_Seperate_Carrier() = 0;
    virtual void received_COMMAND_Reset_Telemetry_Number() = 0;
    virtual void received_COMMAND_Set_ManuelThrust_off() = 0;
    virtual void received_COMMAND_Set_ManuelThrust_on() = 0;


signals:
    void send(QByteArray data, uint16_t receiver);


public slots:
    //PACKAGE TYPES

    void received_DATA(QByteArray DATA);
    void received_COMMAND(uint8_t COMMAND);
};

#endif // ABSTRACTTELEMETRYOBJECT_H
