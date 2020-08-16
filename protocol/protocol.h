#ifndef _PROTOCOL_
#define _PROTOCOL_

#include <stdint.h>



#define TX_REQUEST_16       0x1
#define RX_RESPONSE_16      0x81
#define TX_STATUS_RESPONSE  0x89
#define NO_FRAME            0xFF

#define START_DELIMITER     0x7E

#define MAX_LEN 85 // I select a sufficently large number as max, real max for xbee is different


typedef struct __attribute__((packed)) Header{
    uint8_t startDelimiter;
    uint8_t lenght_high = 0;
    uint8_t lenght_low;
    uint8_t frameType = 0xFF;

}  Header;


//16-bit Transmit Request - 0x01
typedef struct __attribute__((packed)) Transmit_Request_16{
    Header header;
    uint8_t frameID = 0;
    uint8_t dest_addr_high = 0;
    uint8_t dest_addr_low;
    uint8_t options = 0;
    uint8_t data[MAX_LEN];
} Transmit_Request_16;

//Transmit Status - 0x89
typedef struct __attribute__((packed)) Transmit_Status{
    Header header;
    uint8_t frameID;
    uint8_t deliveryStatus;
    uint8_t checksum;
} Transmit_Status;

/*
 * Complete list of delivery statuses:
0x00 = Success
0x01 = No ACK received
0x02 = CCA failure
0x03 = Indirect message unrequested
0x04 = Transceiver was unable to complete the transmission
0x21 = Network ACK failure
0x22 = Not joined to network
0x2C = Invalid frame values (check the phone number)
0x31 = Internal error
0x32 = Resource error - lack of free buffers, timers, etc.
0x34 = No Secure Session Connection
0x35 = Encryption Failure
0x74 = Message too long
0x76 = Socket closed unexpectedly
0x78 = Invalid UDP port
0x79 = Invalid TCP port
0x7A = Invalid host address
0x7B = Invalid data mode
0x7C = Invalid interface. See User Data Relay Input - 0x2D.
0x7D = Interface not accepting frames. See User Data Relay
Input - 0x2D.
0x7E = A modem update is in progress. Try again after the
update is complete.
0x80 = Connection refused
0x81 = Socket connection lost
0x82 = No server
0x83 = Socket closed
0x84 = Unknown server
0x85 = Unknown error
0x86 = Invalid TLS configuration (missing file, and so forth)
0x87 = Socket not connected
0x88 = Socket not bound
Refer to the tables below for a filtered list of status codes that are
appropriate for specific devices.
 * */


typedef struct __attribute__((packed)) Receive_Package_16{
    uint8_t startDelimiter;
    uint8_t lenght_high = 0;
    uint8_t lenght_low;
    uint8_t frameType;
    uint8_t src_addr_high = 0;
    uint8_t src_addr_low;
    uint8_t RSSI;
    uint8_t options;
    uint8_t data[MAX_LEN];
}  Receive_Package_16;


#endif //_PROTOCOL_
