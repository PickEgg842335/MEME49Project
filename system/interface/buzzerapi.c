#include    "./include/buzzerapi.h"
#include    <stdio.h>
#include    <fcntl.h>
#include    <stdbool.h>

#define DEVICE_NAME         "/dev/buzzer"


void sfBuzzerOutput(unsigned char ubValue)
{
    int wfdBuzzerdev;
    int wRet;

    wfdBuzzerdev = open(DEVICE_NAME, O_WRONLY);
    wRet = write(wfdBuzzerdev, &ubValue, 1);
    close(wfdBuzzerdev);
    if(wRet != 1)
    {
        printf("Write Buzzer fail\n");
        return(-1);
    }
}