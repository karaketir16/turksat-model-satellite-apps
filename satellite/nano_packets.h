#ifndef NANO_PACKETS_H
#define NANO_PACKETS_H

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define NANO_START_DELIMITER 0x7E

typedef struct __attribute__((packed)) nano_package {
    uint8_t startDelimiter;

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

    float voltage;

    uint16_t checksum;
} nano_package;

void calcChecksum(nano_package* np);

uint8_t ckeckChecksum(nano_package* np);


#endif // NANO_PACKETS_H
