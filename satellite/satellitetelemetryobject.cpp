#include "satellitetelemetryobject.h"

#define VIDEO_PATH "/"
#define SAVE_FILE "KaldigimizYerdenDevamiSaglayanDosya.nevfeza"
#define GROUND_FILE "zemin_yukseklik"


#include <math.h>
#include <QDateTime>
#include <math.h>

SatelliteTelemetryObject::SatelliteTelemetryObject()
{
    receiver = 0x003C;
    QObject::connect(this, &SatelliteTelemetryObject::send, &xbeeObj, &mainObj::send);
    QObject::connect(&xbeeObj, &mainObj::receive, this, &SatelliteTelemetryObject::received_DATA);



    //    telemetryFile.set
    telemetryFile.setFileName(QString(VIDEO_PATH) + QDateTime::currentDateTime().toString("ddMMyyyy_hhmmss") + QString(".csv"));

    telemetryFile.open(QIODevice::ReadWrite | QIODevice::Truncate);

    telemetyOutput.setDevice(&telemetryFile);
    telemetyOutput << QString("Takim No,Paket No,Gönderme Saati,Basınç,Yükseklik,İniş Hızı,"
                     "Sıcaklık,Pil Gerilimi,GPS Latitude,GPS Longitude,GPS ALtitude,"
                     "Uydu Statüsü,Pitch,Roll,Yaw,Dönüş Sayısı,Video Aktarım Bilgisi\n");

    telemetyOutput.flush();


    Telemetry_update = {{0,0}, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    Telemetry_update.header.type = Package_Enum::TELEMETRY_DATA;
    Telemetry_update.package_number = 0;
    Telemetry_update.team_no = team_no;
    telemetry_number_counter = 0;

    for(int i = 0; i < 2 ; i++){
        saveValuesFile[i].setFileName(VIDEO_PATH SAVE_FILE + QString::number(i));
        saveValuesFile[i].open(QIODevice::ReadWrite);
        QByteArray ba = saveValuesFile[i].readAll();
        if((uint)ba.size() >= sizeof (SaveValues)){
            SaveValues sv;
            auto cnt = ba.size() / sizeof (SaveValues);
            memcpy(&sv, ba.data() + sizeof (SaveValues) * (cnt - 1), sizeof (SaveValues));
            Telemetry_update.package_number = sv.package_number;
            telemetry_number_counter = sv.telemetry_number;
            Telemetry_update.status = sv.status;
            groundHeight = sv.groundHeight;
        }
        else {
            //First start, default values
        }
        saveValuesFile[i].close();
    }

    qDebug() << "read: " << currentThreadId();





    //    Telemetry_sendThis.day = 0x01;
    //    Telemetry_sendThis.month = 0x02;
    //    Telemetry_sendThis.gps_latitude = 4000.3;
    //    Telemetry_sendThis.gps_longtitude = 2100.3;
        QTimer::singleShot(0,this,&SatelliteTelemetryObject::loop);


        uint16_t status = dof.begin(dof.G_SCALE_2000DPS,
                                    dof.A_SCALE_16G, dof.M_SCALE_2GS,
                                    dof.G_ODR_95_BW_125, dof.A_ODR_400, dof.M_ODR_100, "/dev/i2c-1");

        lsmStatus = (0x49D4 == status);


    //    qDebug() << status;
    //    qDebug() << "Should be " << 0x49D4;
    //    Q_ASSERT(status == 0x49D4);
        QTimer tm;
        tm.setInterval(75);
        tm.start();
        QObject::connect(&tm, &QTimer::timeout,[&](){
            QElapsedTimer timer;
            timer.start();
            lsm9ds0_readAll();

            Telemetry_update.yaw = lsm9ds0_getHeading();
            Telemetry_update.roll = lsm9ds0_getRoll();
            Telemetry_update.pitch = lsm9ds0_getPitch();
    //        qDebug() << "Heading, roll, pitch" << Telemetry_update.yaw << Telemetry_update.roll << Telemetry_update.pitch;
        });



        telemSet.setInterval(1000);
        telemSet.start();
        telemetrySetter();
        QObject::connect(&telemSet, &QTimer::timeout,this, &SatelliteTelemetryObject::telemetrySetter);

        QTimer::singleShot(1200,[&](){
            telemetrySender();
        });

}

void SatelliteTelemetryObject::run(){




    this->exec();
}

void SatelliteTelemetryObject::pressDvrSaveButton(){
    QProcess::execute("gpio -g write 18 1");
    QProcess::execute("gpio -g write 18 0");
    QTimer::singleShot(150, [&](){
            QProcess::execute("gpio -g write 18 1");
    });

    qDebug() << "DVR button pressed";
}

void MOTOR_ARM(){
    QProcess::execute("gpio -g pwm 13 90");
}


void SatelliteTelemetryObject::writeSaveValues(){
    saveValues.status = Telemetry_update.status;
    saveValues.package_number = Telemetry_update.package_number;
    saveValues.telemetry_number = telemetry_number_counter;
    saveValues.groundHeight = groundHeight;
    if(saveValues == saveValuesWritten) return;
    for(int i =0; i < 2; i++){
        saveValuesFile[i].open(QIODevice::ReadWrite | QIODevice::Truncate);
//        saveValuesFile[i].resize(0);
    //    saveValuesFile.seek(0);
        QByteArray ba(sizeof (saveValues), 0x00);
        memcpy(ba.data(), &saveValues, sizeof (saveValues));
        saveValuesFile[i].write(ba);
        saveValuesFile[i].flush();
        saveValuesFile[i].close();
    }
    saveValuesWritten = saveValues;
    qDebug() << "write: " << currentThreadId();
}

void SatelliteTelemetryObject::addPressure(float hPa){
    pressureValues.push_back(hPa);
    if(pressureValues.size() > 10){
        pressureValues.pop_front();
    }
}
float SatelliteTelemetryObject::getAveragePressure(){
    float tmp = 0;
    for(auto &a: pressureValues){
        tmp += a;
    }
    tmp /= pressureValues.size();
    return tmp;
}

void SatelliteTelemetryObject::addVoltage(float V){
    voltageValues.push_back(V);
    if(voltageValues.size() > 20){
        voltageValues.pop_front();
    }
}
float SatelliteTelemetryObject::getAverageVoltage(){
    float tmp = 0;
    for(auto &a: voltageValues){
        tmp += a;
    }
    tmp /= voltageValues.size();
    return tmp;
}

#define HEIGHT_MOCK
#undef HEIGHT_MOCK

static float MOCK_H = 0;
static float MOCK_I = 0;



float SatelliteTelemetryObject::heightCalculator(float hPa, float p0){
#ifdef HEIGHT_MOCK


    if(MOCK_H - groundHeight > 700 ){
        MOCK_I = -1;
    }

    if(MOCK_I == 1){
        return MOCK_H += (MOCK_I * 15);
    }
    return MOCK_H += (MOCK_I * (rand()%20)/10.0);
#else
    return 44330.0 * (1.0 - pow(hPa / p0, 0.1903));
#endif

}

void SatelliteTelemetryObject::received_Nano_Package(nano_package np){

    /*
    qDebug() << "recevied Lat: " << np.gps_latitude;
    qDebug() << "recevied Long: " << np.gps_longtitude;
    qDebug() << "recevied Alt: " << np.gps_altitude;
    qDebug() << "recevied Fix: " << np.gps_fix;
    qDebug() << "recevied hour: " << np.hour;
    qDebug() << "recevied minute: " << np.minute;
    qDebug() << "recevied seconds: " << np.second;
    qDebug() << "received pressure: " << np.pressure;
    qDebug() << "received voltage: " << np.voltage;
    qDebug() << "Temp : " << np.temp;
    qDebug() << "-----------------";
    */
    Telemetry_update.day = np.day;
    Telemetry_update.hour = np.hour;
    Telemetry_update.year = np.year;
    Telemetry_update.month = np.month;
    Telemetry_update.minute = np.minute;
    Telemetry_update.second = np.second;
    Telemetry_update.gps_altiude = np.gps_altitude;
    Telemetry_update.gps_latitude = np.gps_latitude;
    Telemetry_update.gps_longtitude = np.gps_longtitude;

    addPressure(np.pressure);
    Telemetry_update.pressure = getAveragePressure();
//    qDebug() << pressureValues;
//    qDebug() << getAveragePressure();

    addVoltage(np.voltage);
    Telemetry_update.voltage = getAverageVoltage();

    Telemetry_update.temperature = np.temp;
    Telemetry_update.GPS_fix = np.gps_fix;

//    auto tempTemp = 25.0;

//    auto powed = pow(( / ),(1/5.257)) - 1;


    auto newHeight = heightCalculator(getAveragePressure(), pressure0);

    newHeight -= groundHeight;

    auto lastTime = nanoTimer.restart();
    if(lastTime == 0){
         Telemetry_update.speed = 0;
    }
    else{
        Telemetry_update.speed = ((newHeight - Telemetry_update.height) / (lastTime * 1.0)) * 1000;
    }

    Telemetry_update.height = newHeight;

    maxReachedHeight = std::max(maxReachedHeight, newHeight);


    Telemetry_update.status = calcSatelliteStatus();
}

//Giving high ms will block to program, zero infinite beeps, no block
void SatelliteTelemetryObject::beepBuzzer(int ms){
    QProcess::execute("gpio -g write 17 1");
    if(ms){
        usleep(1000 * ms);
        QProcess::execute("gpio -g write 17 0");
    }
}

uint8_t SatelliteTelemetryObject::calcSatelliteStatus() {
//    static bool something = true;
//    static bool cancel

    switch (Telemetry_update.status) {
        case Status_Enum::NONE:
        break;

        case Status_Enum::Start:
            if(Telemetry_update.height > 40){
                Telemetry_update.status = Status_Enum::Rising_Before_475;
                MOTOR_ARM();
            }
            if(Telemetry_update.height > 475){
                Telemetry_update.status = Status_Enum::Rising_After_475;
                MOTOR_ARM();
            }
        break;

        case Status_Enum::Rising_Before_475:
            if(Telemetry_update.height > 475){
                Telemetry_update.status = Status_Enum::Rising_After_475;
            }
        break;

        case Status_Enum::Rising_After_475:
            if(Telemetry_update.height < maxReachedHeight - 75){
                Telemetry_update.status = Status_Enum::Falling_Before_Sep;
            }
        break;

        case Status_Enum::Falling_Before_Sep:
            if(Telemetry_update.height < 400){
                Telemetry_update.status = Status_Enum::Seperating;
                received_COMMAND_Seperate_Carrier(0);
                QTimer::singleShot(500,[&](){

                    //First Run Motors Here

                    Telemetry_update.status = Status_Enum::Falling_After_Sep;
                });
            }
        break;

        case Status_Enum::Seperating:
            //Nothing to do
        break;

        case Status_Enum::Falling_After_Sep:
            if(Telemetry_update.height < 50){
                Telemetry_update.status = Status_Enum::Slow_Fall;
            }
            else {
                //Set Motor Speed
                if(Telemetry_update.speed < -10){
                    if(settedThrust < 20){
                        settedThrust += 2;
                    }
                    else {
                        settedThrust = 80;
                    }
                } else if(Telemetry_update.speed > -8){
                    if(settedThrust < 20){
                        settedThrust += 2;
                    }
                    else {
                        settedThrust = 50;
                    }
                }
                else {
                    if(settedThrust < 20){
                        settedThrust += 2;
                    }
                    else {
                        settedThrust = 60;
                    }
                }
                received_COMMAND_Set_Thrust(settedThrust);
                qDebug() << "Setted Thrust: " << settedThrust;
            }
        break;

        case Status_Enum::Slow_Fall:
            if(Telemetry_update.height < 5){

                //Turn Off motors
                settedThrust = 0;
                received_COMMAND_Set_Thrust(settedThrust);
                qDebug() << "Motors Stop: " << settedThrust;

                //Run Buzzer Here
                beepBuzzer(0);


                Telemetry_update.status = Status_Enum::Ground_After_Fall;

                qDebug() << "DVR will pressed in one minute";
                QTimer::singleShot(60 * 1000, [&](){
                    pressDvrSaveButton();
                });
            }
            else {
                //Set Motor Speed For slow fall
                if(Telemetry_update.speed < -3){
                    settedThrust = 80;
                } else if(Telemetry_update.speed > -1){
                    settedThrust = 50;
                }
                else {
                    settedThrust = 60;
                }
                received_COMMAND_Set_Thrust(settedThrust);
                qDebug() << "Setted Thrust slow: " << settedThrust;
            }
        break;

        case Status_Enum::Ground_After_Fall:
            //Running Buzzer, never end ?
        break;


        case Status_Enum::End:
        break;
    }

    writeSaveValues();

    return Telemetry_update.status;
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
    videoFile.setFileName(QString(VIDEO_PATH) + QString("_")+QDateTime::currentDateTime().toString("ddMMyyyy_hhmmss") + QString((char*)set_video_name.name)); //TODO video PATH
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
        qDebug() << "Duplicate VIDEO package: " << video_data.video_packet_number;
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


void SatelliteTelemetryObject::received_COMMAND_Altitude_Calibrate(uint8_t data){
//    groundAltitude = Telemetry_update.gps_altiude;
//    groundPressure = Telemetry_update.pressure;
    groundHeight = heightCalculator(getAveragePressure(), pressure0);
    Telemetry_update.height  = 0;
    MOCK_I = 1;
    maxReachedHeight = 0;
    Telemetry_update.status = Status_Enum::Start;
    writeSaveValues();
}
void SatelliteTelemetryObject::received_COMMAND_Seperate_Carrier(uint8_t data){
//    QProcess::execute("onion pwm 1 13.7 100");
    QString str = QString("gpio -g pwm ") + QString(" 12 ") + QString::number((int) SEPERATOR_SEPERATED);
    QProcess::execute(str);
//    Q_ASSERT(false);
//    usleep(10000000000);

}
void SatelliteTelemetryObject::received_COMMAND_Reset_Telemetry_Number(uint8_t data){
    telemetry_number_counter = 0;
    writeSaveValues();
    //TODO do file operations
}
void SatelliteTelemetryObject::received_COMMAND_Reset_Package_Number(uint8_t data){
    Telemetry_update.package_number = 0;
    writeSaveValues();
    //TODO do file operations
}
void SatelliteTelemetryObject::received_COMMAND_Reset_Satellite_Status(uint8_t data){
//    Telemetry_update.package_number = 0;
    Telemetry_update.status = Status_Enum::NONE;
    writeSaveValues();
    //TODO do file operations
}
void SatelliteTelemetryObject::received_COMMAND_Set_ManuelThrust_off(uint8_t data){
    manuelThrust = false;
}
void SatelliteTelemetryObject::received_COMMAND_Set_ManuelThrust_on(uint8_t data){
    manuelThrust = true;
}
void SatelliteTelemetryObject::received_COMMAND_Set_Thrust(uint8_t data){
    auto step = (MAX_THRUST - MIN_THRUST) / 100.0;
    auto res = MIN_THRUST + (step * data);
    QString str = QString("gpio -g pwm ") + QString(" 13 ") + QString::number((int) res);

    QProcess::execute(str);
}

uint32_t SatelliteTelemetryObject::generateTelemetryNumber(){
    telemetry_number_counter++;
    writeSaveValues();
    return telemetry_number_counter;
//    return telemetry_number_counter++;
}

void SatelliteTelemetryObject::received_COMMAND_Test_Thrust(uint8_t data){

//    Q_ASSERT(false);
    MOTOR_ARM();
    QTimer::singleShot(2000, [&](){
        QString str = QString("gpio -g pwm ") + QString(" 13 ") + QString::number((int) TEST_THRUST_0);
        QProcess::execute(str);
        QTimer::singleShot(500, [&](){
            QString str = QString("gpio -g pwm ") + QString(" 13 ") + QString::number((int) TEST_THRUST_1);
            QProcess::execute(str);

            QTimer::singleShot(10000, [&](){
                QString str = QString("gpio -g pwm ") + QString(" 13 ") + QString::number((int) MIN_THRUST);
                QProcess::execute(str);
            });
        });
    });
}

void SatelliteTelemetryObject::received_COMMAND_Set_Seperator(uint8_t data){
    auto step = (SEPERATOR_SEPERATED - SEPERATOR_NOT_SEPERATED) / 100.0;
    auto res = SEPERATOR_NOT_SEPERATED + (step * data);
//     = "onion pwm 1 " + QString::number(res,'f',2) + " 100";
    QString str = QString("gpio -g pwm ") + QString(" 12 ") + QString::number((int) res);
    QProcess::execute(str);
    qDebug() <<"data: " << data << "#### " << str;
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
    writeSaveValues();
    Telemetry_sendThis = to_byte_array(&Telemetry_update, sizeof (Telemetry_update));
    setTelemetryNumber(Telemetry_sendThis);
    crc_fill((uint8_t *)Telemetry_sendThis.data(),Telemetry_sendThis.size());


    const auto &data = Telemetry_update;


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
}

void SatelliteTelemetryObject::telemetrySender(){
    reSender(Telemetry_sendThis, false, false);

    QTimer::singleShot(50,[&](){
        telemetrySender();
    });
}
