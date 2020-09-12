#include "telemetry_protocol.h"

QVector<QString> Status_Text = {
    "Beklemede",
    "Başlangıç",
    "Yükselme, 400 metre öncesi",
    "Yükselme, 400 metre sonrası",
    "Alçalma, Ayrılma Öncesi" ,
    "Alçalma, Ayrılma",
    "Alçalma, Ayrılma Sonrası",
    "Alçalma, Yavaşlama",
    "Yer Seviyesinde",
    "",
    ""};


bool operator==(const SaveValues a, const SaveValues b){
    return a.status == b.status && a.package_number == b.package_number && a.telemetry_number == b.telemetry_number;
}


// crc functions retrieved from a project of `Huseyin Kozan`
// https://github.com/huseyinkozan

uint8_t crc_is_valid(uint8_t * buff, uint8_t len) {

    uint8_t ck_a = 0, ck_b = 0;
    uint8_t i_a = len-2, i_b = len-1;
    for (uint8_t i = 0; i < len-2; i++) {
      ck_a = (ck_a + buff[i]) & 0xFF;
      ck_b = (ck_a + ck_b) & 0xFF;
    }
    return (buff[i_a] == ck_a && buff[i_b] == ck_b) ? 1 : 0;
}
void crc_fill(uint8_t * buff, uint8_t len) {
    uint8_t ck_a = 0, ck_b = 0;
    uint32_t i_a = len-2, i_b = len-1;
    for (uint8_t i = 0; i < len-2; i++) {
      ck_a = (ck_a + buff[i]) & 0xFF;
      ck_b = (ck_a + ck_b) & 0xFF;
    }
    buff[i_a] = ck_a;
    buff[i_b] = ck_b;
}
