#ifndef I2C_H
#define I2C_H
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <sys/ioctl.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <stdint.h>


int8_t setSlaveAddress(int file, uint8_t addr);

int I2C_init(const char * path);
void I2C_deinit(int file);

int8_t i2c_smbus_access(int file, char read_write, __u8 command,
                         int size, union i2c_smbus_data *data, uint8_t pec);

int8_t i2c_smbus_read_word_data(int file, uint8_t slaveAddress, __u8 command,
                                 uint8_t* data ,__u8 pec);

int8_t i2c_smbus_write_word_data(int file, uint8_t slaveAddress, __u8 command,
                                  uint8_t* data , __u8 pec);

int8_t i2c_read_buff(int file, uint8_t addr, uint8_t reg_addr, uint8_t* data, uint8_t len);

int8_t i2c_write_buff(int file, uint8_t addr, uint8_t* data, uint8_t len);

#endif // I2C_H
