#include "nano_packets.h"

void calcChecksum(nano_package* np){
    np->checksum = 0;
    int len = sizeof (nano_package);
    uint8_t * data = (uint8_t *) np;

    uint8_t tot=0;
    for(int i =0; i < len; i++){
        tot += data[i];
    }
    np->checksum = 0xFF - tot;
}

uint8_t ckeckChecksum(nano_package* np){
    int len = sizeof (nano_package);
    uint8_t * data = (uint8_t *) np;

    uint8_t tot=0;
    for(int i =0; i < len; i++){
        tot += data[i];
    }
    return tot == 0xFF;
}
