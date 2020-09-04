#include "protocol.h"

#include <QByteArray>

#include <QPair>

QPair<Transmit_Request_16, uint32_t> create_Transmit_Request_16(QByteArray &data, uint16_t receiver){

    uint8_t sum = 0;
    Transmit_Request_16 tx;
    auto len = data.size() + 5;

    tx.header.startDelimiter = 0x7E;
    tx.header.lenght_high = 0xFF & (len >> 8);
    tx.header.lenght_low = 0xFF & len;

    sum += tx.header.frameType = TX_REQUEST_16;
    sum += tx.frameID = 0x01;
    sum += tx.dest_addr_high = 0xFF & (receiver >> 8);
    sum += tx.dest_addr_low = 0xFF & receiver;
    sum += tx.options = 0x00;


    memcpy(tx.data, data.data(), data.size());

    int i = 0;
    for(; i < data.size(); i++){
        sum += tx.data[i];
    }
    tx.data[i] = 0xFF - sum;
    return qMakePair(tx, len + 4);
}

QByteArray get_data_Receive_Package_16(Receive_Package_16 &rx, bool &stat, uint8_t *RSSI){

    uint16_t len = rx.lenght_high;
    len <<= 8;
    len += rx.lenght_low;

    if(len > MAX_LEN){
        stat = false;
        return QByteArray();
    }

    if(RSSI)
        *RSSI = rx.RSSI;

    len += 4;

    uint8_t sum = 0;
    QByteArray bytes(len, 0x00);
    memcpy(bytes.data(), &rx, len);
    for(int i = 3; i < len; i++){
        sum += bytes.data()[i];
    }
    if(sum == 0xFF){
        stat = true;
        bytes.remove(0,8);
        bytes.chop(1);
        return bytes;
    }

    stat = false;
    return QByteArray();
}

QByteArray to_byte_array(void* ptr, uint32_t size) {
    QByteArray data(size, 0x00);
    memcpy(data.data(), ptr, size);
    return data;
}

