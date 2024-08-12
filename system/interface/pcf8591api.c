#include    "./include/pcf8591api.h"
#include    <linux/i2c.h>
#include    <linux/i2c-dev.h>
#include    <sys/ioctl.h>
#include    <stdbool.h>
#include    <stdio.h>
#include    <fcntl.h>
#include    <string.h>
#include    <unistd.h>
#include    <malloc.h>

#define DEVICE_I2C1                     "/dev/i2c-1"
#define DEVICE_ADDRESS                  0x48

#define CONTROLBYTE_CH0                 (0 << 0)
#define CONTROLBYTE_CH1                 (1 << 0)
#define CONTROLBYTE_CH2                 (2 << 0)
#define CONTROLBYTE_CH3                 (3 << 0)
#define CONTROLBYTE_AUTOINCR            (1 << 2)
#define CONTROLBYTE_SINGLEENDED         (0 << 4)
#define CONTROLBYTE_DIFF                (1 << 4)
#define CONTROLBYTE_SINGLEENDED_DIFF    (2 << 4)
#define CONTROLBYTE_TWO_DIFF            (3 << 4)
#define CONTROLBYTE_ENBDAC              (1 << 6)


#define I2C_WRITE_FLAG                  0x00
#define I2C_READ_FLAG                   0x01

unsigned char ubbuf2[5] = {0};


void sfOutputDacValue(unsigned char ubValue)
{
    int wfdi2cdev;
    unsigned char ubControlByte = 0x00;

    ubControlByte |= CONTROLBYTE_ENBDAC;
    wfdi2cdev = open(DEVICE_I2C1, O_RDWR);
    ubbuf2[0] = ubValue;
    ubbuf2[1] = ubValue;
    sfpcf8591_write_bytes(wfdi2cdev, ubControlByte, ubbuf2, 2);
    close(wfdi2cdev);
}


void sfpcf8591_write_bytes(int fd, unsigned char address, unsigned char* data, unsigned short len)
{
    struct i2c_rdwr_ioctl_data fpcf8591_write_data;
    
    fpcf8591_write_data.nmsgs = 1;
    fpcf8591_write_data.msgs = malloc(sizeof(struct i2c_msg) * fpcf8591_write_data.nmsgs);
    
    fpcf8591_write_data.msgs[0].addr = DEVICE_ADDRESS;
    fpcf8591_write_data.msgs[0].flags = I2C_WRITE_FLAG;
    fpcf8591_write_data.msgs[0].len = len + 1; /*address data*/
    fpcf8591_write_data.msgs[0].buf = malloc(fpcf8591_write_data.msgs[0].len);
    
    fpcf8591_write_data.msgs[0].buf[0] = address;
    
    memcpy(&(fpcf8591_write_data.msgs[0].buf[1]), data, (size_t)len);
    
    /* Using ioctl to write data */
    ioctl(fd, I2C_RDWR, (unsigned long)&fpcf8591_write_data);
    
    if(fpcf8591_write_data.msgs != NULL)
    {
        free(fpcf8591_write_data.msgs);
        fpcf8591_write_data.msgs = NULL;
    }
}


void fpcf8591_read_bytes(int fd, unsigned char address, unsigned char* buf, unsigned short len)
{        
    struct i2c_rdwr_ioctl_data fpcf8591_read_bytes;
    
    fpcf8591_read_bytes.nmsgs = 2; /*Need writing address first, then reading*/
    fpcf8591_read_bytes.msgs = malloc(sizeof(struct i2c_msg) * fpcf8591_read_bytes.nmsgs);
    
    fpcf8591_read_bytes.msgs[0].addr = DEVICE_ADDRESS;
    fpcf8591_read_bytes.msgs[0].flags = I2C_WRITE_FLAG;
    fpcf8591_read_bytes.msgs[0].len = 1;
    fpcf8591_read_bytes.msgs[0].buf = malloc(fpcf8591_read_bytes.msgs[0].len);
    
    fpcf8591_read_bytes.msgs[0].buf[0] = address;
    
    fpcf8591_read_bytes.msgs[1].addr = DEVICE_ADDRESS;
    fpcf8591_read_bytes.msgs[1].flags = I2C_READ_FLAG;
    fpcf8591_read_bytes.msgs[1].len = len;
    fpcf8591_read_bytes.msgs[1].buf = malloc(fpcf8591_read_bytes.msgs[0].len);
    fpcf8591_read_bytes.msgs[1].buf[0] = 0x00;
    
    /* Using ioctl to read data */
    ioctl(fd, I2C_RDWR, (unsigned long)&fpcf8591_read_bytes);
    
    memcpy((void*)buf, (void*)(fpcf8591_read_bytes.msgs[1].buf), (unsigned int)len);
    
    if(fpcf8591_read_bytes.msgs != NULL)
    {
        free(fpcf8591_read_bytes.msgs);
        fpcf8591_read_bytes.msgs = NULL;
    }
}
