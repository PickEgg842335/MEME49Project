#include    "./include/app.h"
#include    "../interface/include/dht11api.h"
#include    <stdio.h>

float   fEnvirHumidity = 0;
float   fEnvirTemp = 0;

void sfenvirTask(void)
{
    int     wRet;
    float   fDht11Data[2];

    if((sfGetTaskFlag() & (1 << cEnvirTask)) == (1 << cEnvirTask))
    {
        sfClearTaskFlag(cEnvirTask);
        wRet = swfGetDht11HumidityTemparture(fDht11Data);
        fEnvirHumidity = fDht11Data[0];
        fEnvirTemp = fDht11Data[1];
    }
}


float sffGetEnvirHumidity(void)
{
    return(fEnvirHumidity);
}


float sffGetEnvirTemp(void)
{
    return(fEnvirTemp);
}