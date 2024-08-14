#include    "./include/app.h"
#include    "../interface/include/hchr04api.h"
#include    "../interface/include/colorledapi.h"
#include    <stdio.h>
#include    <stdbool.h>

#define cT(x)       ((x * 1000) / cGarsigTimeCount)
#define cDisCm(x)   (x * 100)

#define cDisCmRedLevel              cDisCm(30)
#define cDisCmYellowLevel           cDisCm(40)

#define FourPcsLED_BLUE             0
#define FourPcsLED_LED_GREEN        1
#define FourPcsLED_LED_YELLOW       2
#define FourPcsLED_LED_RED          3

unsigned int    uwObDistence = 0;
unsigned char   ubObDisLvTrigerFlag = 0x00;

void    sfinitialgarsig(void)
{
    sfSetLedOutput(FourPcsLED_BLUE, 0);
    sfSetLedOutput(FourPcsLED_LED_GREEN, 0);
    sfSetLedOutput(FourPcsLED_LED_YELLOW, 0);
    sfSetLedOutput(FourPcsLED_LED_RED, 0);
    sfTrigerDistenMeasure();
    ubObDisLvTrigerFlag = 0;
}


void    sfexitgarsig(void)
{
    sfSetLedOutput(FourPcsLED_BLUE, 0);
    sfSetLedOutput(FourPcsLED_LED_GREEN, 0);
    sfSetLedOutput(FourPcsLED_LED_YELLOW, 0);
    sfSetLedOutput(FourPcsLED_LED_RED, 0);
    ubObDisLvTrigerFlag = 0;
}


void    sfgarsigTask(void)
{
    if((sfGetTaskFlag() & (1 << cGarsigTask)) == (1 << cGarsigTask))
    {
        sfClearTaskFlag(cGarsigTask);
        sfgarsigwork();
        sfTrigerDistenMeasure();
    }
}


void    sfgarsigwork(void)
{
    static int  wholdYellowCnt = 0;
    static int  wholdRedwCnt = 0;

    uwObDistence = sfuwGetDistenceMmValue();
    if((uwObDistence <= cDisCmYellowLevel) 
        && ((ubObDisLvTrigerFlag & (1 << FourPcsLED_LED_YELLOW)) != (1 << FourPcsLED_LED_YELLOW)))
    {
        if(++wholdYellowCnt >= cT(0.2))
        {
            ubObDisLvTrigerFlag |= (1 << FourPcsLED_LED_YELLOW);
            wholdYellowCnt = 0;
        }
    }
    else if((uwObDistence >= (cDisCmYellowLevel + cDisCm(2))) 
        && ((ubObDisLvTrigerFlag & (1 << FourPcsLED_LED_YELLOW)) == (1 << FourPcsLED_LED_YELLOW)))
    {
        if(++wholdYellowCnt >= cT(30))
        {
            ubObDisLvTrigerFlag &= ~(1 << FourPcsLED_LED_YELLOW);
        }
    }
    else
    {
        if(wholdYellowCnt > 0)
        {
            wholdYellowCnt--;
        }
    }

    if((uwObDistence <= cDisCmRedLevel) 
        && ((ubObDisLvTrigerFlag & (1 << FourPcsLED_LED_RED)) != (1 << FourPcsLED_LED_RED)))
    {
        if(++wholdRedwCnt >= cT(0.2))
        {
            ubObDisLvTrigerFlag |= (1 << FourPcsLED_LED_RED);
        }
    }
    else if((uwObDistence >= (cDisCmRedLevel + cDisCm(2))) 
        && ((ubObDisLvTrigerFlag & (1 << FourPcsLED_LED_RED)) == (1 << FourPcsLED_LED_RED)))
    {
        if(++wholdRedwCnt >= cT(30))
        {
            ubObDisLvTrigerFlag &= ~(1 << FourPcsLED_LED_RED);
        }
    }
    else
    {
        if(wholdRedwCnt > 0)
        {
            wholdRedwCnt--;
        }
    }

    if((ubObDisLvTrigerFlag & (1 << FourPcsLED_LED_RED)) == (1 << FourPcsLED_LED_RED))
    {
        sfSetLedOutput(FourPcsLED_LED_GREEN, 0);
        sfSetLedOutput(FourPcsLED_LED_YELLOW, 0);
        sfSetLedOutput(FourPcsLED_LED_RED, 1);
    }
    else if((ubObDisLvTrigerFlag & (1 << FourPcsLED_LED_YELLOW)) == (1 << FourPcsLED_LED_YELLOW))
    {
        sfSetLedOutput(FourPcsLED_LED_GREEN, 0);
        sfSetLedOutput(FourPcsLED_LED_YELLOW, 1);
        sfSetLedOutput(FourPcsLED_LED_RED, 0);
    }
    else
    {
        sfSetLedOutput(FourPcsLED_LED_GREEN, 1);
        sfSetLedOutput(FourPcsLED_LED_YELLOW, 0);
        sfSetLedOutput(FourPcsLED_LED_RED, 0);
    }
}


unsigned char sfubGetObDisLvTrigerLv(void)
{
    return(ubObDisLvTrigerFlag);
}