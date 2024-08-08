#include "./include/adxl485api.h"

#define DEVICE_I2C1                     "/dev/i2c-1"
#define DEVICE_ADDRESS                  0x53

#define REGISTER_DEVICE_ID              0x00
#define REGISTER_DEVICE_POWER_CTL       0x2D
#define REGISTER_DEVICE_DATA_FORMAT     0x31
#define REGISTER_DEVICE_DATAX0          0x32
#define REGISTER_DEVICE_DATAX1          0x33
#define REGISTER_DEVICE_DATAY0          0x34
#define REGISTER_DEVICE_DATAY1          0x35
#define REGISTER_DEVICE_DATAZ0          0x36
#define REGISTER_DEVICE_DATAZ1          0x37

#define I2C_WRITE_FLAG 0x00
#define I2C_READ_FLAG 0x01

unsigned char ubbuf[256] = {0};

void sfinitaladxl485(void)
{
    int wfdi2cdev;

    wfdi2cdev = open(DEVICE_I2C1, O_RDWR);
    ubbuf[0] = 0x0B;
    i2c_write_bytes(wfdi2cdev, REGISTER_DEVICE_DATA_FORMAT, ubbuf, 1);
    ubbuf[0] = 0x08;
    i2c_write_bytes(wfdi2cdev, REGISTER_DEVICE_POWER_CTL, ubbuf, 1);
    close(wfdi2cdev);
}


unsigned char sfubGetadxl485DeviceId(void)
{
    int wfdi2cdev;
    unsigned char ubDeviceId = 0;

    wfdi2cdev = open(DEVICE_I2C1, O_RDWR);
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_ID, ubbuf, 1);
    ubDeviceId = ubbuf[0];
    close(wfdi2cdev);
    return(ubDeviceId);
}


int sfwGetadxl485DataX(void)
{
    int wfdi2cdev;
    short sdata = 0;
    int wResult = 0;

    wfdi2cdev = open(DEVICE_I2C1, O_RDWR);
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_DATAX1, ubbuf, 1);
    sdata = (short)ubbuf[0] << 8;
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_DATAX0, ubbuf, 1);
    sdata |= (short)ubbuf[0];
    wResult = (int)sdata;
    close(wfdi2cdev);
    return(wResult);
}


int sfwGetadxl485DataY(void)
{
    int wfdi2cdev;
    short sdata = 0;
    int wResult = 0;

    wfdi2cdev = open(DEVICE_I2C1, O_RDWR);
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_DATAY1, ubbuf, 1);
    sdata = (short)ubbuf[0] << 8;
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_DATAY0, ubbuf, 1);
    sdata |= (short)ubbuf[0];
    wResult = (int)sdata;
    close(wfdi2cdev);
    return(wResult);
}


int sfwGetadxl485DataZ(void)
{
    int wfdi2cdev;
    short sdata = 0;
    int wResult = 0;

    wfdi2cdev = open(DEVICE_I2C1, O_RDWR);
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_DATAZ1, ubbuf, 1);
    sdata = (short)ubbuf[0] << 8;
    i2c_read_bytes(wfdi2cdev, REGISTER_DEVICE_DATAZ0, ubbuf, 1);
    sdata |= (short)ubbuf[0];
    wResult = (int)sdata;
    close(wfdi2cdev);
    return(wResult);
}


void i2c_write_bytes(int fd, unsigned char address, unsigned char* data, unsigned short len)
{
    struct i2c_rdwr_ioctl_data adxl485_write_data;
    
    adxl485_write_data.nmsgs = 1;
    adxl485_write_data.msgs = malloc(sizeof(struct i2c_msg) * adxl485_write_data.nmsgs);
    
    adxl485_write_data.msgs[0].addr = DEVICE_ADDRESS;
    adxl485_write_data.msgs[0].flags = I2C_WRITE_FLAG;
    adxl485_write_data.msgs[0].len = len + 1; /*address data*/
    adxl485_write_data.msgs[0].buf = malloc(adxl485_write_data.msgs[0].len);
    
    adxl485_write_data.msgs[0].buf[0] = address;
    
    memcpy(&(adxl485_write_data.msgs[0].buf[1]), data, (size_t)len);
    
    /* Using ioctl to write data */
    ioctl(fd, I2C_RDWR, (unsigned long)&adxl485_write_data);
    
    printf("Write data success\n");
    
    if(adxl485_write_data.msgs != NULL)
    {
        free(adxl485_write_data.msgs);
        adxl485_write_data.msgs = NULL;
    }
}


void i2c_read_bytes(int fd, unsigned char address, unsigned char* buf, unsigned short len)
{        
    struct i2c_rdwr_ioctl_data adxl485_read_data;
    
    adxl485_read_data.nmsgs = 2; /*Need writing address first, then reading*/
    adxl485_read_data.msgs = malloc(sizeof(struct i2c_msg) * adxl485_read_data.nmsgs);
    
    adxl485_read_data.msgs[0].addr = DEVICE_ADDRESS;
    adxl485_read_data.msgs[0].flags = I2C_WRITE_FLAG;
    adxl485_read_data.msgs[0].len = 1;
    adxl485_read_data.msgs[0].buf = malloc(adxl485_read_data.msgs[0].len);
    
    adxl485_read_data.msgs[0].buf[0] = address;
    
    adxl485_read_data.msgs[1].addr = DEVICE_ADDRESS;
    adxl485_read_data.msgs[1].flags = I2C_READ_FLAG;
    adxl485_read_data.msgs[1].len = len;
    adxl485_read_data.msgs[1].buf = malloc(adxl485_read_data.msgs[0].len);
    adxl485_read_data.msgs[1].buf[0] = 0x00;
    
    /* Using ioctl to read data */
    ioctl(fd, I2C_RDWR, (unsigned long)&adxl485_read_data);
    
    /*printf("e2prom_read_data.msgs[1].buf[0] = 0x%x\n", adxl485_read_data.msgs[1].buf[0]);*/
    
    memcpy((void*)buf, (void*)(adxl485_read_data.msgs[1].buf), (unsigned int)len);
    
    if(adxl485_read_data.msgs != NULL)
    {
        free(adxl485_read_data.msgs);
        adxl485_read_data.msgs = NULL;
    }
}
