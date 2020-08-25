#ifndef BME280_WRAPPER_H
#define BME280_WRAPPER_H


#include <stdint.h>

typedef struct bme_File_and_Addr{
    int file;
    uint8_t addr;
} bme_File_and_Addr;

extern bme_File_and_Addr bme_file_and_addr;

void user_delay_us(uint32_t period, void *intf_ptr);

int8_t user_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

int8_t user_spi_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

#endif // BME280_WRAPPER_H
