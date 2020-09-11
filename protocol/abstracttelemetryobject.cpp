#include "abstracttelemetryobject.h"
#include "QDebug"
#include "QTimer"
AbstractTelemetryObject::AbstractTelemetryObject(QObject *parent) : QThread(parent)
{

}

//uint32_t AbstractTelemetryObject::sender(QByteArray data, uint8_t resend){ // old function
//    Telemetry_Header * hd = (Telemetry_Header *)data.data();
//    if( ! resend)

////    qDebug() << "resend: " << resend <<" counter: "<< telemetry_number_counter;

////    qDebug() << "hex: " << data.toHex();
////    qDebug() << "before send check: " << crc_is_valid((uint8_t*)data.data(), data.size());
//    emit send(data, receiver);
//    return hd->telemetry_number;
//}

uint32_t AbstractTelemetryObject::setTelemetryNumber(QByteArray &data){
    Telemetry_Header * hd = (Telemetry_Header *)data.data();
    hd->telemetry_number = telemetry_number_counter++;
    return  hd->telemetry_number;
}

uint32_t AbstractTelemetryObject::reSender(QByteArray data, uint8_t resend, uint8_t firstTime){
    Telemetry_Header * hd = (Telemetry_Header *)data.data();
    if(firstTime){ // if data will send first time, set a number and calculate crc
        setTelemetryNumber(data);
        crc_fill((uint8_t*)data.data(), data.size());
    }

    if(ackedPackages.contains(hd->telemetry_number)){ // if received ack remove from buffer
        bufferedData.remove(hd->telemetry_number);
        bufferedNumbers.remove(hd->telemetry_number);
    }
    else { // {re}send data
        if(resend){
            bufferedData[hd->telemetry_number] = data; // buffer data
            bufferedNumbers.insert(hd->telemetry_number);

            QTimer::singleShot(30, [this,data](){ // resend
                reSender(data, true, false);
//                qDebug() << "Resend";
            });
        }

        emit send(data, receiver);
    }
    return hd->telemetry_number;
}


void AbstractTelemetryObject::received_PACKAGE_ACK(ACK ack){
    qDebug() << "Get ACK: "<< ack.ack_telemetry_number;
    ackedPackages.insert(ack.ack_telemetry_number);
}


void AbstractTelemetryObject::received_DATA(QByteArray DATA){

//    qDebug() <<"gelen DATA: " << DATA.toHex();
    uint8_t checkCRC = crc_is_valid((uint8_t*) DATA.data(), DATA.size());
    if( ! checkCRC){
        qDebug() << "CRC Error";
        return;
    }

//    qDebug() <<"DATA: " << DATA.toHex();
    Telemetry_Header * header =(Telemetry_Header *) DATA.data();

    if( header->type == Package_Enum::COMMAND || header->type == Package_Enum::SET_VIDEO_NAME){
        ACK tmp; //({{0, PACKAGE_ACK},header->telemetry_number,0,0});
        tmp.header.type = Package_Enum::ACK_DATA;
        tmp.ack_telemetry_number = header->telemetry_number;
//        emit send(to_byte_array(&tmp, sizeof(ACK)), receiver);
        reSender(to_byte_array(&tmp, sizeof(ACK)),false, true);
    } // send ACK

    if(header->type == Package_Enum::VIDEO_DATA){
        Video_Data * vd = (Video_Data *) DATA.data();
        Video_Data_ACK tmp;//({{0,PACKAGE_Video_Data_ACK}, vd->video_packet_number,0,0});
        tmp.header.type = Package_Enum::VIDEO_DATA_ACK;
        tmp.video_packet_number = vd->video_packet_number;
        //emit send(to_byte_array(&tmp, sizeof(Video_Data_ACK)), receiver);
        reSender(to_byte_array(&tmp, sizeof(Video_Data_ACK)), false, true);
     }
//    else{
//        return;
//    }
    // send video data ack

    if(     header->type != Package_Enum::ACK_DATA &&
            header->type != Package_Enum::VIDEO_DATA_ACK &&
            receivedNumbers.contains(header->telemetry_number))
    {
        // same package already received
//        qDebug() << "duplicate receive";
    }else{
        receivedNumbers.insert(header->telemetry_number);
        switch (header->type) {
            case Package_Enum::TELEMETRY_DATA:
                received_PACKAGE_Telemetry_Data(*((Telemetry_Data*)DATA.data()));
            break;
            case Package_Enum::ACK_DATA:
                received_PACKAGE_ACK(*((ACK*)DATA.data()));
            break;
            case Package_Enum::COMMAND:
                received_COMMAND(((Command*)DATA.data())->command, ((Command*)DATA.data())->data);
            break;
            case Package_Enum::SET_VIDEO_NAME:
                received_PACKAGE_Set_Video_Name(*((Set_Video_Name*)DATA.data()));
            break;
            case Package_Enum::VIDEO_DATA:
                received_PACKAGE_Video_Data(*((Video_Data*)DATA.data()));
            break;
            case Package_Enum::VIDEO_DATA_ACK:
                received_PACKAGE_Video_Data_ACK(*((Video_Data_ACK*)DATA.data()));
            break;
        }
    }
}


void AbstractTelemetryObject::received_COMMAND(uint8_t COMMAND, uint8_t data){
    switch (COMMAND) {
        case Command_Enum::ALTITUDE_CALIBRATE:
            received_COMMAND_Altitude_Calibrate(data);
        break;
        case Command_Enum::SEPERATE_CARRIER:
            received_COMMAND_Seperate_Carrier(data);
        break;
        case Command_Enum::RESET_TELEMETRY_NUMBER:
            received_COMMAND_Reset_Telemetry_Number(data);
        break;
        case Command_Enum::SET_MANUELTHRUST_OFF:
            received_COMMAND_Set_ManuelThrust_off(data);
        break;
        case Command_Enum::SET_MANUELTHRUST_ON:
            received_COMMAND_Set_ManuelThrust_on(data);
        break;
        case Command_Enum::SET_THRUST:
            received_COMMAND_Set_Thrust(data);
        break;
        case Command_Enum::SET_SEPERATOR:
            received_COMMAND_Set_Seperator(data);
        break;
        case Command_Enum::TEST_THRUST:
            received_COMMAND_Test_Thrust(data);
        break;
    }
}

