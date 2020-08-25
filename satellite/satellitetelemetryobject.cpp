#include "satellitetelemetryobject.h"

SatelliteTelemetryObject::SatelliteTelemetryObject()
{

}

void SatelliteTelemetryObject::run(){
    Telemetry_sendThis.header.type = PACKAGE_Telemetry_Data;
    Telemetry_sendThis.day = 0x01;
    Telemetry_sendThis.month = 0x02;
    Telemetry_sendThis.gps_latitude = 4000.3;
    Telemetry_sendThis.gps_longtitude = 2100.3;
    QTimer::singleShot(0,this,&SatelliteTelemetryObject::loop);
    this->exec();
}


void SatelliteTelemetryObject::received_PACKAGE_Telemetry_Data(Telemetry_Data data){

}
void SatelliteTelemetryObject::received_PACKAGE_ACK(ACK ack){

}
void SatelliteTelemetryObject::received_PACKAGE_Command (Command cmd){

}
void SatelliteTelemetryObject::received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name){

}
void SatelliteTelemetryObject::received_PACKAGE_Video_Data(Video_Data video_data){

}
void SatelliteTelemetryObject::received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK){

}


void SatelliteTelemetryObject::received_COMMAND_Altitude_Calibrate(){

}
void SatelliteTelemetryObject::received_COMMAND_Seperate_Carrier(){

}
void SatelliteTelemetryObject::received_COMMAND_Reset_Telemetry_Number(){

}
void SatelliteTelemetryObject::received_COMMAND_Set_ManuelThrust_off(){

}
void SatelliteTelemetryObject::received_COMMAND_Set_ManuelThrust_on(){

}

void SatelliteTelemetryObject::loop(){
    qDebug() << "loop";

    QByteArray toSent(sizeof (Telemetry_Data), 0x00);
    memcpy(toSent.data(), &Telemetry_sendThis,sizeof (Telemetry_Data));


    emit send(toSent,receiver);


    QTimer::singleShot(0,this,&SatelliteTelemetryObject::loop);
}
