#include "abstracttelemetryobject.h"
#include "QDebug"
AbstractTelemetryObject::AbstractTelemetryObject(QObject *parent) : QThread(parent)
{

}

void AbstractTelemetryObject::received_DATA(QByteArray DATA){
    qDebug() <<"DATA: " << DATA.toHex();
    Telemetry_Header * header =(Telemetry_Header *) DATA.data();
    switch (header->type) {
        case PACKAGE_Telemetry_Data:
            received_PACKAGE_Telemetry_Data(*((Telemetry_Data*)DATA.data()));
        break;
        case PACKAGE_ACK:
            received_PACKAGE_ACK(*((ACK*)DATA.data()));
        break;
        case PACKAGE_Command:
            received_PACKAGE_Command(*((Command*)DATA.data()));
        break;
        case PACKAGE_Set_Video_Name:
            received_PACKAGE_Set_Video_Name(*((Set_Video_Name*)DATA.data()));
        break;
        case PACKAGE_Video_Data:
            received_PACKAGE_Video_Data(*((Video_Data*)DATA.data()));
        break;
        case PACKAGE_Video_Data_ACK:
            received_PACKAGE_Video_Data_ACK(*((Video_Data_ACK*)DATA.data()));
        break;
    }
}


void AbstractTelemetryObject::received_COMMAND(uint8_t COMMAND){
    switch (COMMAND) {
        case COMMAND_Altitude_Calibrate:
            received_COMMAND_Altitude_Calibrate();
        break;
        case COMMAND_Seperate_Carrier:
            received_COMMAND_Seperate_Carrier();
        break;
        case COMMAND_Reset_Telemetry_Number:
            received_COMMAND_Reset_Telemetry_Number();
        break;
        case COMMAND_Set_ManuelThrust_off:
            received_COMMAND_Set_ManuelThrust_off();
        break;
        case COMMAND_Set_ManuelThrust_on:
            received_COMMAND_Set_ManuelThrust_on();
        break;
    }
}
