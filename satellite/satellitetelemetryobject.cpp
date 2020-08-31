#include "satellitetelemetryobject.h"

#define VIDEO_PATH "/mnt/mmcblk0p1/"

SatelliteTelemetryObject::SatelliteTelemetryObject()
{
    receiver = 0x0037;
}

void SatelliteTelemetryObject::run(){
    Telemetry_update = {{0,0}, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    Telemetry_update.header.type = PACKAGE_Telemetry_Data;
    Telemetry_update.package_number = 0;
    Telemetry_update.team_no = team_no;
    telemetry_number_counter = 0;

//    Telemetry_sendThis.day = 0x01;
//    Telemetry_sendThis.month = 0x02;
//    Telemetry_sendThis.gps_latitude = 4000.3;
//    Telemetry_sendThis.gps_longtitude = 2100.3;
    QTimer::singleShot(0,this,&SatelliteTelemetryObject::loop);


    uint16_t status = dof.begin(dof.G_SCALE_2000DPS,
                                dof.A_SCALE_16G, dof.M_SCALE_2GS,
                                dof.G_ODR_95_BW_125, dof.A_ODR_400, dof.M_ODR_100);

    lsmStatus = (0x49D4 == status);


//    qDebug() << status;
//    qDebug() << "Should be " << 0x49D4;
//    Q_ASSERT(status == 0x49D4);
    QTimer tm;
    tm.setInterval(200);
    tm.start();
    QObject::connect(&tm, &QTimer::timeout,[&](){
        QElapsedTimer timer;
        timer.start();
        lsm9ds0_readAll();

        Telemetry_update.yaw = lsm9ds0_getHeading();
        Telemetry_update.roll = lsm9ds0_getRoll();
        Telemetry_update.pitch = lsm9ds0_getPitch();
    });


    QTimer telemSet;
    telemSet.setInterval(1000);
    telemSet.start();
    telemetrySetter();
    QObject::connect(&telemSet, &QTimer::timeout,this, &SatelliteTelemetryObject::telemetrySetter);

    QTimer::singleShot(1200,[&](){
        telemetrySender();
    });

    this->exec();
}

void SatelliteTelemetryObject::received_Nano_Package(nano_package np){
    qDebug() << "recevied Lat: " << np.gps_latitude;
    qDebug() << "recevied Long: " << np.gps_longtitude;
    qDebug() << "recevied Alt: " << np.gps_altitude;
    qDebug() << "recevied Fix: " << np.gps_fix;
    qDebug() << "recevied hour: " << np.hour;
    qDebug() << "recevied minute: " << np.minute;
    qDebug() << "recevied seconds: " << np.second;
    qDebug() << "received pressure: " << np.pressure;
    qDebug() << "received voltage: " << np.voltage;
    qDebug() << "-----------------";
    Telemetry_update.day = np.day;
    Telemetry_update.hour = np.hour;
    Telemetry_update.year = np.year;
    Telemetry_update.month = np.month;
    Telemetry_update.minute = np.minute;
    Telemetry_update.second = np.second;
    Telemetry_update.gps_altiude = np.gps_altitude;
    Telemetry_update.gps_latitude = np.gps_latitude;
    Telemetry_update.gps_longtitude = np.gps_longtitude;
    Telemetry_update.pressure = np.pressure;
    Telemetry_update.voltage = np.voltage;
    Telemetry_update.temperature = np.temp;
    Telemetry_update.GPS_fix = np.gps_fix;
}


void SatelliteTelemetryObject::received_PACKAGE_Telemetry_Data(Telemetry_Data data){
    //Not implemented
    Q_UNUSED(data);
}
//void SatelliteTelemetryObject::received_PACKAGE_ACK(ACK ack){
//    //Not implemented
//    Q_UNUSED(ack);
//}

void SatelliteTelemetryObject::received_PACKAGE_Set_Video_Name(Set_Video_Name set_video_name){
    expectedVideoParts = set_video_name.video_packet_count;
    videoData = QVector<QByteArray>(expectedVideoParts);
    videoFile.setFileName(QString(VIDEO_PATH) + QString((char*)set_video_name.name)); //TODO video PATH
    if( ! videoFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){
        qDebug() << "Cannot Open";
    }
    qDebug() << "Set Video Name: " << QString((char*)set_video_name.name);

    testTimer.start();
//    ACK ack;
//    ack.header.type = PACKAGE_ACK;
//    ack.header.telemetry_number = 0;
//    ack.ack_telemetry_number =
//    send(to_byte_array())
}
void SatelliteTelemetryObject::received_PACKAGE_Video_Data(Video_Data video_data){
//    qDebug() <<"video pac number: " << video_data.video_packet_number;
//    qDebug() << "A";
//    if(video_data.video_packet_number >= expectedVideoParts){

//    }
//    qDebug() << "B";
    if(videoData[video_data.video_packet_number].size() != 0){
        //duplicate Video package
//        qDebug() << "Duplicate VIDEO package: " << video_data.video_packet_number;
    }
    else {
        QByteArray tmp(video_data.video_data_len,0x00);
        memcpy(tmp.data(), video_data.video_data, video_data.video_data_len);
        videoData[video_data.video_packet_number] = tmp;
    }
//    qDebug() << "C";

//    qDebug() << "getted video data: " << tmp.toHex();



    while(videoWritePointer < expectedVideoParts && videoData[videoWritePointer].size() != 0){
        videoFile.write(videoData[videoWritePointer++]);
    }
//    qDebug() << "D";


    videoFile.flush();
    if(videoWritePointer == expectedVideoParts){
        Telemetry_update.video_check = true;
    } else{
//        qDebug() << "write Pointer" << videoWritePointer <<" nextSize: " << videoData[videoWritePointer].size();
    }
    Telemetry_update.videoPercent = 100 * videoWritePointer / (expectedVideoParts * 1.0);
//    qDebug() << "E";
}
void SatelliteTelemetryObject::received_PACKAGE_Video_Data_ACK(Video_Data_ACK video_data_ACK){
    //Not Implemented
    Q_UNUSED(video_data_ACK);
}


void SatelliteTelemetryObject::received_COMMAND_Altitude_Calibrate(){
    groundAltitude = Telemetry_update.gps_altiude;
    groundPressure = Telemetry_update.pressure;
}
void SatelliteTelemetryObject::received_COMMAND_Seperate_Carrier(){
    QProcess::execute("onion pwm 1 13.7 100");
//    Q_ASSERT(false);
//    usleep(10000000000);

}
void SatelliteTelemetryObject::received_COMMAND_Reset_Telemetry_Number(){
    telemetry_number_counter = 0;
    //TODO do file operations
}
void SatelliteTelemetryObject::received_COMMAND_Reset_Package_Number(){
    Telemetry_update.package_number = 0;
    //TODO do file operations
}
void SatelliteTelemetryObject::received_COMMAND_Set_ManuelThrust_off(){
    manuelThrust = false;
}
void SatelliteTelemetryObject::received_COMMAND_Set_ManuelThrust_on(){
    manuelThrust = true;
}

void SatelliteTelemetryObject::loop(){ //Update to toSent



//    QByteArray toSent(sizeof (Telemetry_Data), 0x00);
//    memcpy(toSent.data(), &Telemetry_sendThis,sizeof (Telemetry_Data));


//    reSender(toSent, false, true);

    if(expectedVideoParts){
        qDebug() << "Video percentage: " << 100 * videoWritePointer / (expectedVideoParts * 1.0);
        qDebug() << "time: "<< testTimer.elapsed() / 1000;
    }else{
        qDebug() << "Video Not Started";
    }


    QTimer::singleShot(1000,this,&SatelliteTelemetryObject::loop);
}

void SatelliteTelemetryObject::telemetrySetter(){
    Telemetry_update.package_number++; //increase number every second
    Telemetry_sendThis = to_byte_array(&Telemetry_update, sizeof (Telemetry_update));
    setTelemetryNumber(Telemetry_sendThis);
    crc_fill((uint8_t *)Telemetry_sendThis.data(),Telemetry_sendThis.size());
}

void SatelliteTelemetryObject::telemetrySender(){
    reSender(Telemetry_sendThis, false, false);

    QTimer::singleShot(50,[&](){
        telemetrySender();
    });
}
