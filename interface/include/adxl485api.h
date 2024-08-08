#include    <linux/i2c.h>
#include    <linux/i2c-dev.h>
#include    <sys/ioctl.h>
#include    <stdbool.h>
#include    <stdio.h>
#include    <fcntl.h>
#include    <string.h>
#include    <unistd.h>
#include    <malloc.h>

extern void sfinitaladxl485(void);
extern unsigned char sfubGetadxl485DeviceId(void);
extern int sfwGetadxl485DataX(void);
extern int sfwGetadxl485DataY(void);
extern int sfwGetadxl485DataZ(void);