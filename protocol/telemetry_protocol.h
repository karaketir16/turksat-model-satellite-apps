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


//PACKAGE TYPES

#define PACKAGE_Telemetry_Data          0x01
#define PACKAGE_ACK                     0x02
#define PACKAGE_Command                 0x03
#define PACKAGE_Set_Video_Name          0x04
#define PACKAGE_Video_Data              0x05
#define PACKAGE_Video_Data_ACK          0x06

#define COMMAND_Altitude_Calibrate      0x21
#define COMMAND_Seperate_Carrier        0x22
#define COMMAND_Reset_Telemetry_Number  0x23
#define COMMAND_Set_ManuelThrust_off    0x24
#define COMMAND_Set_ManuelThrust_on     0x25


#define STATUS_                         0x41

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
    uint16_t pressure; // Pa
    uint16_t height; // meter
    uint16_t speed; // m/s
    uint8_t temperature; //Celcius
    uint8_t voltage;
    float gps_latitude;
    float gps_longtitude;
    float gps_altiude;

    uint8_t status;
    float pitch;
    float roll;
    float yaw;

    uint16_t rotation_count;
    uint8_t video_check;

//    uint8_t checksum;
} Telemetry_Data;

typedef struct __attribute__((packed)) ACK {
    Telemetry_Header header;
    uint32_t ack_package_number;
//    uint8_t checksum;
} ACK;

typedef struct __attribute__((packed)) Command {
    Telemetry_Header header;
    uint8_t command;
//    uint8_t checksum;
} Command;

typedef struct __attribute__((packed)) Set_Video_Name {
    Telemetry_Header header;
    uint8_t data[25]; //what if video name larger then 25 ????
    uint16_t video_packet_count;
//    uint8_t checksum;
} Set_Video_Name;

typedef struct __attribute__((packed)) Video_Data {
    Telemetry_Header header;
    uint16_t video_packet_number;
    uint8_t video_data[50];
//    uint8_t checksum;
} Video_Data;

typedef struct __attribute__((packed)) Video_Data_ACK {
    Telemetry_Header header;
    uint16_t video_packet_number;
//    uint8_t checksum;
} Video_Data_ACK;

typedef struct __attribute__((packed)) Set_Thrust {
    Telemetry_Header header;
    uint8_t thrust;
//    uint8_t checksum;
} Set_Thrust;


#endif // TELEMETRY_PROTOCOL_H
