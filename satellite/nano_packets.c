#include "nano_packets.h"

void calcChecksum(nano_package* np){
    np->checksum = 0;
    int len = sizeof (nano_package);
    uint8_t * data = (uint8_t *) np;

    uint16_t tot=0;
    int i =0;
    for(; i < len; i++){
        tot += data[i];
    }
    np->checksum = 0xFFFF - tot;
}


uint8_t ckeckChecksum(nano_package* np){
    int len = sizeof (nano_package);
    uint8_t * data = (uint8_t *) np;

    uint16_t tot=0;
    int i =0;
    for(; i < len - 2; i++){
        tot += data[i];
    }
    return tot == np-> checksum;
}
