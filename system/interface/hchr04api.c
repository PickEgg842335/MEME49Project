#include    "./include/hchr04api.h"
#include    <stdio.h>
#include    <fcntl.h>

#define DEVICE_NAME         "/dev/hchr04"


void    sfTrigerDistenMeasure(void)
{
    int wfddev;
    int wRet;
    unsigned char ubValue = 1;

    wfddev = open(DEVICE_NAME, O_WRONLY);
    wRet = write(wfddev, &ubValue, 1);
    if(wRet != 1)
    {
        printf("HC-HR04 trig fail\n");
    }
    close(wfddev);
}


unsigned int    sfuwGetDistenceMmValue(void)
{
    int wfddev;
    int wRet;
    unsigned char bValue[9];
    unsigned int uwdistenceCnt = 0;

    wfddev = open(DEVICE_NAME, O_RDONLY);
    wRet = read(wfddev, &bValue, sizeof(bValue));
    if(wRet != 0)
    {
        printf("HC-HR04 read fail\n");
    }
    close(wfddev);

    uwdistenceCnt = ((unsigned int)bValue[7] << 8) | (unsigned int)bValue[8];
    uwdistenceCnt = uwdistenceCnt * 100 / 58;

    return(uwdistenceCnt);
}
