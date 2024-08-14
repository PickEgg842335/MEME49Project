#include    "./include/app.h"
#include    "../interface/include/dht11api.h"
#include    "../interface/include/colorledapi.h"
#include    <stdio.h>
#include    <stdbool.h>

#define cT(x) ((x * 1000) / cEnvirTimeCount)

#define FourPcsLED_LED_BLUE             0

float           fEnvirHumidity = 0;
float           fEnvirTemp = 0;
unsigned char   ubEnvLightValue = 0;
unsigned char   ubEnvLightLevel = 3;
unsigned char   ubEnvLightStatus = 0;


void sfenvirTask(void)
{
    if((sfGetTaskFlag() & (1 << cEnvirTask)) == (1 << cEnvirTask))
    {
        sfClearTaskFlag(cEnvirTask);
        sfInputAdcValueUpgrade();
        sfMeasureEnvirData();
        sfLightControl();
    }
}


void sfMeasureEnvirData(void)
{
    int     wRet;
    float   fDht11Data[2];

    wRet = swfGetDht11HumidityTemparture(fDht11Data);
    fEnvirHumidity = fDht11Data[0];
    fEnvirTemp = fDht11Data[1];
    ubEnvLightValue = sfubGetAdcChannelValue(0);
    ubEnvLightLevel = (((unsigned int)ubEnvLightValue * 5) >> 8) + 1; 
}


void sfLightControl(void)
{
    static unsigned int uwdelayOffLightCnt = 0;

    if(ubEnvLightLevel <= 2)
    {
        sfSetLedOutput(FourPcsLED_LED_BLUE, 1);
        uwdelayOffLightCnt = cT(60);
        ubEnvLightStatus = true;
    }
    
    if(uwdelayOffLightCnt > 0)
    {
        uwdelayOffLightCnt--;
    }
    else
    {
        sfSetLedOutput(FourPcsLED_LED_BLUE, 0);
        ubEnvLightStatus = false;
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


unsigned char sfubGetEnvirLightValuePer(void)
{
    return((unsigned char)(((unsigned int)ubEnvLightValue * 100) >> 8) + 1);
}


unsigned char sfubGetEnvirLightLevel(void)
{
    return(ubEnvLightLevel);
}


unsigned char sfubGetEnvirLightStatus(void)
{
    return(ubEnvLightStatus);
}
