#include    "./include/app.h"
#include    "../interface/include/buzzerapi.h"
#include    <stdbool.h>

#define cT(x) ((x * 1000) / cBuzzerTimeCount)
#define cBUZZER_FREQ   25

unsigned char ubBuzzerStatus = false;

void sfexitbuzzertask(void)
{
    sfBuzzerOnOff(false);
}

void sfbuzzerTask(void)
{
    if((sfGetTaskFlag() & (1 << cBuzzerTask)) == (1 << cBuzzerTask))
    {
        sfClearTaskFlag(cBuzzerTask);
        sfBuzzerWorkControl();
    }
}


void sfBuzzerWorkControl(void)
{
    static unsigned int uwTimeCnt = 0;
    static unsigned char ubBuzzerData = false;

    if(sfwGetItityLvTrigerFlag() != 0)
    {
        sfBuzzerOnOff(true);
    }
    else if(sfubGetObDisLvTrigerLv() != 0)
    {
        sfBuzzerOnOff(true);
    }
    else
    {
        sfBuzzerOnOff(false);
    }
}


void sfBuzzerOnOff(unsigned char ubFlag)
{
    static unsigned int uwPeriodCnt = 0;

    if(ubFlag == true)
    {
        if(uwPeriodCnt >= cBUZZER_FREQ)
        {
            sfBuzzerOutput(1);
            uwPeriodCnt = 0;
        }
        else if(uwPeriodCnt >= (cBUZZER_FREQ >> 1))
        {
            sfBuzzerOutput(0);
            uwPeriodCnt++;
        }
        else
        {
            sfBuzzerOutput(1);
            uwPeriodCnt++;
        }
        ubBuzzerStatus = true;
    }
    else
    {
        sfBuzzerOutput(0);
        uwPeriodCnt = 0;
        ubBuzzerStatus = false;
    }
}


unsigned char sfubGetBuzzerStatus(void)
{
    return(ubBuzzerStatus);
}