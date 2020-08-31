#ifndef ABSTRACTTELEMETRYOBJECT_H
#define ABSTRACTTELEMETRYOBJECT_H

#include <QObject>
#include <QByteArray>
#include <QThread>
#include "telemetry_protocol.h"
#include <QSet>
#include <QMap>
class AbstractTelemetryObject : public QThread
{
    Q_OBJECT

public:
    explicit AbstractTelemetryObject(QObject *parent = nullptr);


    void received_PACKAGE_ACK(ACK ack);

    virtual void received_PACKAGE_Telemetry_Data(Telemetry_Data data) = 0;


    virtual void received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name) = 0;
    virtual void received_PACKAGE_Video_Data(Video_Data video_data) = 0;
    virtual void received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK) = 0;


    virtual void received_COMMAND_Altitude_Calibrate() = 0;
    virtual void received_COMMAND_Seperate_Carrier() = 0;
    virtual void received_COMMAND_Reset_Telemetry_Number() = 0;
    virtual void received_COMMAND_Reset_Package_Number() = 0;
    virtual void received_COMMAND_Set_ManuelThrust_off() = 0;
    virtual void received_COMMAND_Set_ManuelThrust_on() = 0;

//    uint32_t sender(QByteArray data, uint8_t resend = false); // returns telemetry number
    uint32_t reSender(QByteArray data, uint8_t resend = true, uint8_t firstTime = true);
    uint32_t setTelemetryNumber(QByteArray &data);

    uint32_t telemetry_number_counter = 1;

//    ACK createAck(uint32_t telemetry_number);

    QSet<uint32_t> receivedNumbers;
    QSet<uint32_t> bufferedNumbers;
    QMap<uint32_t, QByteArray> bufferedData;

    QSet<uint32_t> ackedPackages;

    uint16_t receiver = 0xABAB;

signals:
    void send(QByteArray data, uint16_t receiver);


public slots:
    //PACKAGE TYPES

    void received_DATA(QByteArray DATA);
    void received_COMMAND(uint8_t COMMAND);
};

#endif // ABSTRACTTELEMETRYOBJECT_H
