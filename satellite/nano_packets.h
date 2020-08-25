#ifndef NANO_PACKETS_H
#define NANO_PACKETS_H

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct __attribute__((packed)) nano_package {
    float gps_longtitude;
    float gps_altitude;
    float gps_latitude;

    uint8_t gps_fix;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    uint8_t day;
    uint8_t month;
    uint16_t year;

    float pressure;
    float temp;

    uint16_t checksum;
} nano_package;



#endif // NANO_PACKETS_H
