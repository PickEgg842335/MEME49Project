#include "./include/app.h"


void    sfinitialws2812bdispaly(void)
{
    sfOpenWbs2812bLedEnable(true);
    sfSetWbs2812bQty(2);
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
    unsigned char ubRBBData[2][3] = {{0, 0, 0} , {0, 0, 0}};

    switch(wLevel)
    {
        case 0:
            ubRBBData[0][0] = 0;
            ubRBBData[0][1] = 255;
            ubRBBData[0][2] = 0;
        break;

        case 1:
            ubRBBData[0][0] = 255;
            ubRBBData[0][1] = 255;
            ubRBBData[0][2] = 0;
        break;

        case 2:
            ubRBBData[0][0] = 255;
            ubRBBData[0][1] = 0;
            ubRBBData[0][2] = 0;
        break;
    }

    sfOpenWbs2812bLedEnable(true);
    sfSetWbs2812bRGBData(1,ubRBBData[0]);
    sfSetWbs2812bRGBData(2,ubRBBData[1]);
    sfSetWbs2812bStartDisplay();
    sfOpenWbs2812bLedEnable(false);
}