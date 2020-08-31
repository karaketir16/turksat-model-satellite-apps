#include "stationtelemetryobject.h"
#include <QFile>
#include <string.h>
#include <QFileInfo>
StationTelemetryObject::StationTelemetryObject()
{
    receiver = 0x003C;
}

void StationTelemetryObject::run(){

    QTimer::singleShot(500, [&](){
        videoReadyChecker();
    });

    this->exec();
}

#define PARTSIZE 50 //TODO

void StationTelemetryObject::sendSeperateCarrier(){
    Command cmd;
    cmd.header.type = PACKAGE_Command;
    cmd.command = COMMAND_Seperate_Carrier;
//    Q_ASSERT(false);
    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
    qDebug() <<"carrier Send";
//    Q_ASSERT(false);
}

void StationTelemetryObject::videoReadyChecker(){
    if( ! this->videoReady){
        QTimer::singleShot(500, [&](){
            videoReadyChecker();
        });
    }
    else {
        QTimer::singleShot(0, [&](){
            videoSender(true);
        });
    }
    //not send
}

void StationTelemetryObject::videoSender(bool firstTime){


//    return;

    if(firstTime){
        qDebug() <<"FileName: " << videoPath;
        QFile file(videoPath);
        if( ! file.open(QIODevice::ReadOnly)){
            qDebug() << "Cannot Open";
        }
        videoData = file.readAll();
        videoSize = videoData.size();



        for(uint64_t i =0; i < videoSize; i+=PARTSIZE)
        {
            auto sendThat = videoData.mid(i, PARTSIZE);
            videoPackets.push_back(sendThat);
        }

        videoPartCount = videoPackets.size();
//        ackedVideoParts = QVector<uint8_t>(videoPartCount, false);
        partPointer = 0;

        Set_Video_Name svn;
        svn.header.type = PACKAGE_Set_Video_Name;
        videoPath = QFileInfo(file).fileName();
        Q_ASSERT(videoPath.size() < 25);
        strcpy((char *)svn.name, videoPath.toStdString().c_str());
        svn.video_packet_count = videoPartCount;

        videoSetTelemNumber = reSender(to_byte_array(&svn, sizeof (svn)));

        videoSenderIndex = 0;
    }

    if(videoNameSetted || ackedPackages.contains(videoSetTelemNumber)){
        videoNameSetted = true;
        while(lastPoint < videoPartCount && ackedVideoParts.contains(lastPoint)){
            lastPoint++;
        }

        if(videoSenderIndex < 1000/*1000*/ && (videoSenderIndex + lastPoint) < videoPartCount){
    //        qDebug() << "index: " << videoSenderIndex;
            auto packet = videoPackets[videoSenderIndex + lastPoint];
            if(ackedVideoParts.contains(videoSenderIndex + lastPoint)){
                // continue
    //            qDebug() << "ACKED: " << videoSenderIndex + lastPoint;
            }
            else{
    //            qDebug() << "SEND : " << videoSenderIndex + lastPoint;
                Video_Data vd;
                vd.header.type = PACKAGE_Video_Data;
                vd.video_data_len = packet.size();
                vd.video_packet_number = videoSenderIndex + lastPoint;

        //            qDebug() << "will send: " << packet.toHex();
                memcpy(vd.video_data, packet.data(), vd.video_data_len);

                reSender(to_byte_array(&vd, sizeof(vd)), false, true);

            }

            videoSenderIndex++;
        }
        else{
            videoSenderIndex = 0;
        }
//        qDebug()



//        for(uint32_t i = 0; i < 50 && (i + lastPoint) < videoPartCount; i++){
//            auto packet = videoPackets[i + lastPoint];
//            if(ackedVideoParts[i + lastPoint])
//                continue;
//            qDebug() << "send: " << i + lastPoint;
//            Video_Data vd;
//            vd.header.type = PACKAGE_Video_Data;
//            vd.video_data_len = packet.size();
//            vd.video_packet_number = i + lastPoint;

////            qDebug() << "will send: " << packet.toHex();
//            memcpy(vd.video_data, packet.data(), vd.video_data_len);

//            reSender(to_byte_array(&vd, sizeof(vd)), false, true);
//        }
    }

    if(lastPoint != videoPartCount){
        QTimer::singleShot(0, [&](){
            videoSender(false);
        });
    }else{
        qDebug() << "Sender Done!";
//        usleep()
    }
}

void StationTelemetryObject::loop(){


    QTimer::singleShot(1, this, &StationTelemetryObject::loop);
}

void StationTelemetryObject::received_PACKAGE_Telemetry_Data(Telemetry_Data data){
    qDebug() <<"+++++++++++++++++++++++++++++++++";
    qDebug() << "Package Number: " << data.package_number;
    qDebug() << "Latitude: " << data.gps_latitude << " Longtitude: " << data.gps_longtitude << " Altitude: " << data.gps_altiude;
    qDebug() << "Date: " << data.day << "/" << data.month << "/" << data.year <<
                " time: " << data.hour << ":" << data.minute << ":" << data.second;
    qDebug() << "Pitch: " << data.pitch << " Roll: " <<data.roll << " Yaw: " << data.yaw;
    qDebug() << "Pressure: " << data.pressure;
    qDebug() << "Video Done: " << (data.video_check ? "OK" : "Not Yet");
    qDebug() << "Voltage: " << data.voltage;
    qDebug() << "Temperature: " << data.temperature;
    qDebug() << "Gps FIX: " << data.GPS_fix;
    qDebug() <<"+++++++++++++++++++++++++++++++++";

    emit newTelemetryData(data);
//    qDebug() <<
    //TODO
}

//void StationTelemetryObject::received_PACKAGE_ACK(ACK ack){

//}
void StationTelemetryObject::received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name){
    //Not Implemented
    Q_UNUSED(set_video_name);
}
void StationTelemetryObject::received_PACKAGE_Video_Data(Video_Data video_data){
    //Not Implemented
    Q_UNUSED(video_data);
}
void StationTelemetryObject::received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK){
    qDebug() << "get video ack: " << video_data_ACK.video_packet_number;
    if(ackedVideoParts.contains(video_data_ACK.video_packet_number)){
        qDebug() << "Duplicate VIDEO ACK: " << video_data_ACK.video_packet_number;
    }
    else{
        ackedVideoParts.insert(video_data_ACK.video_packet_number);
    }
}


void StationTelemetryObject::received_COMMAND_Altitude_Calibrate(){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Seperate_Carrier(){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Reset_Telemetry_Number(){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Reset_Package_Number(){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Set_ManuelThrust_off(){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Set_ManuelThrust_on(){
    //Not Implemented
}
