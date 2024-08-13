#include    "./include/app.h"
#include    "../interface/include/wbs2812bapi.h"
#include    <stdbool.h>

#define cT(x) ((x * 1000) / cWs2812bDisplayTimeCount)

unsigned char ubRBBData[2][3] = {{0, 0, 0} , {0, 0, 0}};

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
        sfWarmingLedDisplay(sfwGetItityLevel());
    }
}


void    sfWarmingLedDisplay(int wLevel)
{
    switch(wLevel)
    {
        case 0:
            sfNormalWorking();
        break;

        case 1:
            sfYellowFlashing();
        break;

        case 2:
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
    ubRBBData[0][1] = 3;
    ubRBBData[0][2] = 0;
    ubRBBData[1][0] = 0;
    ubRBBData[1][1] = 3;
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