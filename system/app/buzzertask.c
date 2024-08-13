#include    "./include/app.h"
#include    "../interface/include/buzzerapi.h"
#include    <stdbool.h>

#define cT(x) ((x * 1000) / cBuzzerTimeCount)
#define cBUZZER_FREQ   2

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

    switch(sfwGetItityLevel())
    {
        case 0:
            sfBuzzerOnOff(false);
        break;
        case 1:
        case 2:
            if(++uwTimeCnt > cT(0.5))
            {
                ubBuzzerData = !ubBuzzerData;
                uwTimeCnt = 0;
            }
            sfBuzzerOnOff(ubBuzzerData);
        break;
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
    }
    else
    {
        sfBuzzerOutput(0);
        uwPeriodCnt = 0;
    }
}