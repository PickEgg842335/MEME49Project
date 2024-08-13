#include    "./include/dht11api.h"
#include    <stdio.h>
#include    <fcntl.h>

#define DEVICE_BLTEST "/dev/dht11"

int swfGetDht11HumidityTemparture(float *ptrArray)
{
    int wfdDth11dev;
    unsigned char ubReadBufTemp[4];
    float fHumidiy, fTemp;
    int wRet;

    wfdDth11dev = open(DEVICE_BLTEST, O_RDONLY);
    if(wfdDth11dev < 0)
    {
        printf("DTH Open fail!!");
        return(-1);
    }

    wRet = read(wfdDth11dev,ubReadBufTemp,sizeof(ubReadBufTemp));
    if(wRet < 0)
    {
        printf("Read Error.");
        return(-1);
    }
    else
    {
        fHumidiy = (float)ubReadBufTemp[0] + ((float)ubReadBufTemp[1] / 10);
        fTemp = (float)ubReadBufTemp[2] + ((float)ubReadBufTemp[3] / 10);

        *ptrArray = fHumidiy;
        *(ptrArray + 1) = fTemp;
    }

    if(wfdDth11dev >= 0)
    {
        close(wfdDth11dev);
    }
    return(0);
}
