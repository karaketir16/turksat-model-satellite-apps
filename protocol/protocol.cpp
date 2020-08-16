#include "protocol.h"

#include <QByteArray>



//void checkSum(QByteArray& arr){
//    uint8_t res = 0;
//    for(int i =3;i<arr.size(); i++)
//    {
//        res += arr.data()[i];
//    }
//    arr.push_back(0xFF - res);
//}

//void packageIT(QByteArray &arr)
//{
//    QByteArray it;
//    it.resize(8);

//    it.data()[0] = 0x7E;
//    it.data()[1] = 0x00;
//    it.data()[2] = arr.size() + 5;
//    it.data()[3] = 0x01; //frame type
//    it.data()[4] = 0x01; //frame ID
//    it.data()[5] = 0x00;
//    it.data()[6] = 0x3C;
//    it.data()[7] = 0x00;
//    it.push_back(arr);
//    checkSum(it);
//    arr = it;
//}
Transmit_Request_16 create_Transmit_Request_16(QByteArray &data, uint16_t receiver){

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
    return tx;
}

QByteArray get_data_Receive_Package_16(Receive_Package_16 &rx, bool &stat){

    uint8_t len = rx.lenght_low;

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
    else {
        stat = false;
        return QByteArray();
    }
}

