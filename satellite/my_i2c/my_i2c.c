#include "my_i2c.h"
#include <unistd.h>

int8_t setSlaveAddress(int file, uint8_t addr)
{
    return ioctl(file, I2C_SLAVE, addr >> 1);
}


int I2C_init(const char* path)
{
    return open(path, O_RDWR);
}

int8_t i2c_read_buff(int file, uint8_t addr, uint8_t reg_addr, uint8_t* data, uint8_t len)
{
    setSlaveAddress(file, addr);
    struct i2c_rdwr_ioctl_data asd;
    asd.msgs = (struct i2c_msg *) (malloc(sizeof(struct i2c_msg)*2));
    struct i2c_msg r,w;
    w.addr = addr >> 1;
    w.flags = 0;
    w.len = 1;
    w.buf = &reg_addr;

    r.addr = addr >> 1;
    r.flags = I2C_M_RD;
    r.len = len;
    r.buf = data;

    asd.msgs[0] = w;
    asd.msgs[1] = r;
    asd.nmsgs = 2;

    return ioctl(file, I2C_RDWR, &asd);
}

int8_t i2c_write_buff(int file, uint8_t addr, uint8_t* data, uint8_t len)
{
    setSlaveAddress(file, addr);
    struct i2c_rdwr_ioctl_data asd;
    asd.msgs = (struct i2c_msg *) (malloc(sizeof(struct i2c_msg)*1));
    struct i2c_msg w;
    w.addr = addr >> 1;
    w.flags = 0;
    w.len = len;
    w.buf = data;

    asd.msgs[0] = w;
    asd.nmsgs = 1;

    return ioctl(file, I2C_RDWR, &asd);
}

int8_t i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data, uint8_t pec)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    ioctl(file,I2C_PEC,pec);
    return ioctl(file,I2C_SMBUS,&args);
}


int8_t i2c_smbus_read_word_data(int file, uint8_t slaveAddress, __u8 command, uint8_t* data ,__u8 pec)
{
    setSlaveAddress(file, slaveAddress);
    union i2c_smbus_data smbus_data;
    if (i2c_smbus_access(file, I2C_SMBUS_READ,command,
                         I2C_SMBUS_WORD_DATA,&smbus_data, pec))
        return -1;
    else
    {
        uint16_t rD = 0xFFFF & smbus_data.word;
        data[0] = (uint8_t)( 0x00FF & rD);
        data[1] = rD >> 8;
        return 0;
    }
}

int8_t i2c_smbus_write_word_data(int file, uint8_t slaveAddress, __u8 command, uint8_t* data , __u8 pec)
{
    setSlaveAddress(file, slaveAddress);
    union i2c_smbus_data smbus_data;
    smbus_data.word = data[1];
    smbus_data.word <<= 8;
    smbus_data.word += data[0];
    return i2c_smbus_access(file, I2C_SMBUS_WRITE,command,
                         I2C_SMBUS_WORD_DATA, &smbus_data, pec);
}


void I2C_deinit(int file)
{
    if (file < 0) {
        close(file);
        file = -1;
    }
}
