#include "stationtelemetryobject.h"
#include <QFile>
#include <string.h>
#include <QFileInfo>
#include <QDateTime>
StationTelemetryObject::StationTelemetryObject()
{
    receiver = 0x0037;
}

void StationTelemetryObject::run(){


//    StationTelemetryObject tmObj;
    QTimer::singleShot(500, [&](){
        videoReadyChecker();
    });

    QTimer::singleShot(0, [&](){
        xBee = new mainObj();
        QObject::connect(this, &StationTelemetryObject::send, xBee, &mainObj::send);
        QObject::connect(xBee, &mainObj::receive, this, &StationTelemetryObject::received_DATA);
    });

    telemetryFile.setFileName(QDateTime::currentDateTime().toString("ddMMyyyy_hhmmss") + QString(".csv"));
    telemetryFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    telemetyOutput.setDevice((&telemetryFile));
    telemetyOutput << QString("Takim No,Paket No,Gönderme Saati,Basınç,Yükseklik,İniş Hızı,"
                     "Sıcaklık,Pil Gerilimi,GPS Latitude,GPS Longitude,GPS ALtitude,"
                     "Uydu Statüsü,Pitch,Roll,Yaw,Dönüş Sayısı,Video Aktarım Bilgisi\n");

    telemetyOutput.flush();
    this->exec();
}

#define PARTSIZE 50 //TODO

//void StationTelemetryObject::sendSeperateCarrier(){
//    Command cmd;
//    cmd.header.type =  Package_Enum::COMMAND;
//    cmd.command = Command_Enum::SEPERATE_CARRIER;
////    Q_ASSERT(false);
//    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
//    qDebug() <<"carrier Send";
////    Q_ASSERT(false);
//}


//void StationTelemetryObject::sendSetSeperator(uint8_t data){
//    Command cmd;
//    cmd.header.type = Package_Enum::COMMAND;
//    cmd.command = Command_Enum::SET_SEPERATOR;
//    cmd.data = data;
//    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
//}
//void StationTelemetryObject::sendSetEngineThrust(uint8_t data){
//    Command cmd;
//    cmd.header.type = Package_Enum::COMMAND;
//    cmd.command = Command_Enum::SET_THRUST;
//    cmd.data = data;
//    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
//}
//void StationTelemetryObject::sendTestThrust(uint8_t data){
//    Command cmd;
//    cmd.header.type = Package_Enum::COMMAND;
//    cmd.command = Command_Enum::TEST_THRUST;
//    cmd.data = data;
//    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
//}

//void StationTelemetryObject::sendGroundSet(uint8_t data){
//    Command cmd;
//    cmd.header.type = Package_Enum::COMMAND;
//    cmd.command = Command_Enum::ALTITUDE_CALIBRATE;
//    cmd.data = data;
//    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
//}

//void StationTelemetryObject::sendResetStatus(uint8_t data){
//    Command cmd;
//    cmd.header.type = Package_Enum::COMMAND;
//    cmd.command = Command_Enum::RESET_SATELLITE_STATUS;
//    cmd.data = data;
//    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
//}

void StationTelemetryObject::commandSend(u_int8_t command, uint8_t data){
    Command cmd;
    cmd.header.type = Package_Enum::COMMAND;
    cmd.command = command;
    cmd.data = data;
    reSender(to_byte_array(&cmd, sizeof (cmd)), true, true);
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
        svn.header.type = Package_Enum::SET_VIDEO_NAME;
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
                qDebug() << "SEND : " << videoSenderIndex + lastPoint;
                Video_Data vd;
                vd.header.type = Package_Enum::VIDEO_DATA;
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
        QTimer::singleShot(10, [&](){
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
    qDebug() << "Latitude: " << QString::number( data.gps_latitude, 'g', 10) << " Longtitude: " << data.gps_longtitude << " Altitude: " << data.gps_altiude;
    qDebug() << "Date: " << data.day << "/" << data.month << "/" << data.year <<
                " time: " << data.hour << ":" << data.minute << ":" << data.second;
    qDebug() << "Pitch: " << data.pitch << " Roll: " <<data.roll << " Yaw: " << data.yaw;
    qDebug() << "Pressure: " << data.pressure;
    qDebug() << "Video Done: " << (data.video_check ? "OK" : "Not Yet");
    qDebug() << "Voltage: " << data.voltage;
    qDebug() << "Temperature: " << data.temperature;
    qDebug() << "Gps FIX: " << data.GPS_fix;
    qDebug() << "Height: " << data.height;
    qDebug() << "Speed: " << data.speed;
    qDebug() <<"+++++++++++++++++++++++++++++++++";

    telemetyOutput << QString("%1,%2,%3,%4,%5,%6,"
                     "%7,%8,%9,%10,%11,"
                     "%12,%13,%14,%15,%16,%17\n")
              .arg(data.team_no)
              .arg(data.package_number)
              .arg(QString("%1/%2/%3 - %4:%5:%6").arg(data.day).arg(data.month).arg(data.year).arg(data.hour).arg(data.minute).arg(data.second))
              .arg(data.pressure)
              .arg(data.height)
              .arg(data.speed)
              .arg(data.temperature)
              .arg(data.voltage)
              .arg(data.gps_latitude)
              .arg(data.gps_longtitude)
              .arg(data.gps_altiude)
              .arg(Status_Text[data.status]) //Status
              .arg(data.pitch)
              .arg(data.roll)
              .arg(data.yaw)
              .arg(data.rotation_count)
              .arg(data.video_check ? "Evet" : "Hayır");

    telemetyOutput.flush();
//    wind->newTelemetryData(data);
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


void StationTelemetryObject::received_COMMAND_Altitude_Calibrate(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Seperate_Carrier(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Reset_Telemetry_Number(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Reset_Package_Number(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Reset_Satellite_Status(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Set_ManuelThrust_off(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Set_ManuelThrust_on(uint8_t data){
    //Not Implemented
}
void StationTelemetryObject::received_COMMAND_Set_Thrust(uint8_t data){

}
void StationTelemetryObject::received_COMMAND_Set_Seperator(uint8_t data){

}

void StationTelemetryObject::received_COMMAND_Test_Thrust(uint8_t){

}

uint32_t StationTelemetryObject::generateTelemetryNumber(){
    return telemetry_number_counter++;
}
