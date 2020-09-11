#ifndef TELEMETRY_PROTOCOL_H
#define TELEMETRY_PROTOCOL_H

// WRITTEN FOR LITTLE_ENDIAN architech
//CHECK
/*
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#endif
*/

#include <stdint.h>
#include "protocol.h"
#include <QVector>
#include <QString>

//PACKAGE TYPES

enum Package_Enum {
    TELEMETRY_DATA  = 0x01  ,
    ACK_DATA                     ,
    COMMAND                 ,
    SET_VIDEO_NAME          ,
    VIDEO_DATA              ,
    VIDEO_DATA_ACK
};




enum Command_Enum {
    ALTITUDE_CALIBRATE = 0x21   ,
    SEPERATE_CARRIER            ,
    RESET_TELEMETRY_NUMBER      ,
    RESET_PACKAGE_NUMBER        ,
    SET_MANUELTHRUST_OFF        ,
    SET_MANUELTHRUST_ON         ,
    SET_THRUST                  ,
    SET_SEPERATOR               ,
    TEST_THRUST                 ,
    CMD__MAX__
};

enum Status_Enum {
    NONE = 0x0          ,
    Start               ,
    Rising_Before_475   ,
    Rising_After_475    ,
    Falling_Before_Sep  ,
    Seperating          ,
    Falling_After_Sep   ,
    Slow_Fall           ,
    Ground_After_Fall   ,
    End                 ,
    STS__MAX__
};

extern QVector<QString> Status_Text ;




#define STATUS_                         0x41


#define MIN_THRUST                      90
#define MAX_THRUST                      190 //can be larger, 21 or 22 not sure

#define TEST_THRUST_0                      102
#define TEST_THRUST_1                      105 //can be larger, 21 or 22 not sure


#define SEPERATOR_SEPERATED                   170
#define SEPERATOR_NOT_SEPERATED               115




typedef struct __attribute__((packed)) Telemetry_Header {
    uint32_t telemetry_number;
    uint8_t type;
} Telemetry_Header;


typedef struct __attribute__((packed)) Telemetry_Data {
    Telemetry_Header header;
    //data

    uint16_t team_no;
    uint16_t package_number;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float pressure; // Pa
    float height; // meter
    float speed; // m/s
    float temperature; //Celcius
    float voltage;
    float gps_latitude;
    float gps_longtitude;
    float gps_altiude;

    uint8_t GPS_fix;

    uint8_t status;
    float pitch;
    float roll;
    float yaw;

    uint16_t rotation_count;
    uint8_t video_check;
    uint8_t videoPercent;

    uint8_t crca;
    uint8_t crcb;
} Telemetry_Data;

typedef struct __attribute__((packed)) ACK {
    Telemetry_Header header;
    uint32_t ack_telemetry_number;
    uint8_t crca;
    uint8_t crcb;
} ACK;

typedef struct __attribute__((packed)) Command {
    Telemetry_Header header;
    uint8_t command;
    uint8_t data;
    uint8_t crca;
    uint8_t crcb;
} Command;

typedef struct __attribute__((packed)) Set_Video_Name {
    Telemetry_Header header;
    uint8_t name[25]; //what if video name larger then 25 ???? //TODO
    uint16_t video_packet_count;
    uint8_t crca;
    uint8_t crcb;
} Set_Video_Name;

typedef struct __attribute__((packed)) Video_Data {
    Telemetry_Header header;
    uint32_t video_packet_number;
    uint8_t video_data_len;
    uint8_t video_data[50];
    uint8_t crca;
    uint8_t crcb;
} Video_Data;

typedef struct __attribute__((packed)) Video_Data_ACK {
    Telemetry_Header header;
    uint32_t video_packet_number;
    uint8_t crca;
    uint8_t crcb;
} Video_Data_ACK;

//typedef struct __attribute__((packed)) Set_Thrust {
//    Telemetry_Header header;
//    uint8_t thrust;
//    uint8_t crca;
//    uint8_t crcb;
//} Set_Thrust;


void crc_fill(uint8_t * buff, uint8_t len);
uint8_t  crc_is_valid(uint8_t * buff, uint8_t len);

#endif // TELEMETRY_PROTOCOL_H
