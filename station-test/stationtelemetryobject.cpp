#include "stationtelemetryobject.h"

StationTelemetryObject::StationTelemetryObject()
{

}

void StationTelemetryObject::run(){
    this->exec();
}


void StationTelemetryObject::received_PACKAGE_Telemetry_Data(Telemetry_Data data){
    qDebug() << "Lat: " << data.gps_latitude << " Long" << data.gps_longtitude;
}

void StationTelemetryObject::received_PACKAGE_ACK(ACK ack){

}
void StationTelemetryObject::received_PACKAGE_Command (Command cmd){

}
void StationTelemetryObject::received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name){

}
void StationTelemetryObject::received_PACKAGE_Video_Data(Video_Data video_data){

}
void StationTelemetryObject::received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK){

}


void StationTelemetryObject::received_COMMAND_Altitude_Calibrate(){

}
void StationTelemetryObject::received_COMMAND_Seperate_Carrier(){

}
void StationTelemetryObject::received_COMMAND_Reset_Telemetry_Number(){

}
void StationTelemetryObject::received_COMMAND_Set_ManuelThrust_off(){

}
void StationTelemetryObject::received_COMMAND_Set_ManuelThrust_on(){

}
