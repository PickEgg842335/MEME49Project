#include    "./include/app.h"
#include    "../interface/include/wbs2812bapi.h"
#include    "../interface/include/pcf8591api.h" //test
#include    <stdbool.h>

#define cT(x) ((x * 1000) / cWs2812bDisplayTimeCount)

unsigned char ubRBBData[2][3] = {{0, 0, 0} , {0, 0, 0}};
unsigned char ubLightValueAdj = 255; 

void    sfinitialws2812bdispaly(void)
{
    sfOpenWbs2812bLedEnable(true);
    sfSetWbs2812bQty(2);
    sfOpenWbs2812bLedEnable(false);
}


void    sfexitws2812bdisplay(void)
{
    ubRBBData[0][0] = 0;
    ubRBBData[0][1] = 0;
    ubRBBData[0][2] = 0;
    ubRBBData[1][0] = 0;
    ubRBBData[1][1] = 0;
    ubRBBData[1][2] = 0;
    sfOpenWbs2812bLedEnable(true);
    sfSetWbs2812bRGBData(1,ubRBBData[0]);
    sfSetWbs2812bRGBData(2,ubRBBData[1]);
    sfSetWbs2812bStartDisplay();
    sfOpenWbs2812bLedEnable(false);
}


void    sfws2812bdisplayTask(void)
{
    if((sfGetTaskFlag() & (1 << cWs2812bDisplayTask)) == (1 << cWs2812bDisplayTask))
    {
        sfClearTaskFlag(cWs2812bDisplayTask);
        sfWarmingLedDisplay(sfwGetItityLvTrigerFlag());
        sfInputAdcValueUpgrade();
        sflightBlackAdj();
    }
}


void    sfWarmingLedDisplay(int wLevel)
{
    switch(wLevel)
    {
        case 0:
            sfNormalWorking();
        break;

        case 0x01:
            sfYellowFlashing();
        break;
        case 0x02:
        case 0x03:
            sfRedFlashing();
        break;
    }

    sfOpenWbs2812bLedEnable(true);
    sfSetWbs2812bRGBData(1,ubRBBData[0]);
    sfSetWbs2812bRGBData(2,ubRBBData[1]);
    sfSetWbs2812bStartDisplay();
    sfOpenWbs2812bLedEnable(false);
}


void    sfNormalWorking(void)
{
    ubRBBData[0][0] = 0;
    ubRBBData[0][1] = ubLightValueAdj | 0x01;
    ubRBBData[0][2] = 0;
    ubRBBData[1][0] = 0;
    ubRBBData[1][1] = ubLightValueAdj | 0x01;
    ubRBBData[1][2] = 0;
}


void    sfYellowFlashing(void)
{
    static unsigned int uwTimeCnt = 0;

    if(uwTimeCnt >= cT(0.5))
    {
        uwTimeCnt = 0;
    }

    if(++uwTimeCnt >= cT(0.25))
    {
        ubRBBData[0][0] = 0;
        ubRBBData[0][1] = 0;
        ubRBBData[0][2] = 0;
        ubRBBData[1][0] = 255;
        ubRBBData[1][1] = 255;
        ubRBBData[1][2] = 0;
    }
    else
    {
        ubRBBData[0][0] = 255;
        ubRBBData[0][1] = 255;
        ubRBBData[0][2] = 0;
        ubRBBData[1][0] = 0;
        ubRBBData[1][1] = 0;
        ubRBBData[1][2] = 0;
    }
}


void    sfRedFlashing(void)
{
    static unsigned int uwTimeCnt = 0;

    if(uwTimeCnt >= cT(0.5))
    {
        uwTimeCnt = 0;
    }

    if(++uwTimeCnt >= cT(0.25))
    {
        ubRBBData[0][0] = 0;
        ubRBBData[0][1] = 0;
        ubRBBData[0][2] = 0;
        ubRBBData[1][0] = 255;
        ubRBBData[1][1] = 0;
        ubRBBData[1][2] = 0;
    }
    else
    {
        ubRBBData[0][0] = 255;
        ubRBBData[0][1] = 0;
        ubRBBData[0][2] = 0;
        ubRBBData[1][0] = 0;
        ubRBBData[1][1] = 0;
        ubRBBData[1][2] = 0;
    }
}


void sflightBlackAdj(void)
{
    static unsigned char ubOutputDataOld = 0;
    ubLightValueAdj = sfubGetDigitalFilter(sfubGetAdcChannelValue(1), ubOutputDataOld);
    ubOutputDataOld = ubLightValueAdj;
}


unsigned char   sfubGetDigitalFilter(unsigned char ubInputData, unsigned char ubOutputDataOld)
{
    unsigned char ubOutputData = 0;
    ubOutputData = (unsigned char)((((unsigned int)ubInputData * 2457) + ((unsigned int)ubOutputDataOld * 1639)) >> 12);
    return(ubOutputData);
}