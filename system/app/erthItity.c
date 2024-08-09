#include "./include/app.h"

#define cT(x) ((x * 1000) / cErthItityTimeCount)

int     wAdcAcceleX = 0;
int     wAdcAcceleY = 0;
int     wAdcAcceleZ = 0;
float   fRealTotalAccele = 0;
float   fRmsTotalAccele = 0;
int     wInityLv = 0;


void sfinitialErthItity(void)
{
    sfinitaladxl485();
}


void sfErthItityTask(void)
{
    if((sfGetTaskFlag() & (1 << cErthItityTask)) == (1 << cErthItityTask))
    {
        sfClearTaskFlag(cErthItityTask);
        sfCalerthItity();
    }
}


void sfCalerthItity(void)
{
    float fTotalAcceleArray[100];
    static float fTemp = 0;
    static unsigned int uwCnt = 0;

    wAdcAcceleX = sfwGetadxl485DataX();
    wAdcAcceleY = sfwGetadxl485DataY();
    wAdcAcceleZ = sfwGetadxl485DataZ();
    fTemp += sqrtf(wAdcAcceleX * wAdcAcceleX + wAdcAcceleY * wAdcAcceleY + wAdcAcceleZ * wAdcAcceleZ) / 256.0;

    if(++uwCnt >= 50)
    {
        fRmsTotalAccele = fTemp / 50.0;
        fTemp = 0;
        uwCnt = 0;

        if(fRmsTotalAccele < 0.25)
        {
            wInityLv = 0;
        }
        else if(fRmsTotalAccele < 4.0)
        {
            wInityLv = 1;
        }
        else
        {
            wInityLv = 2;
        }
    }
}


float sffGetRmsTotalAcccle(void)
{
    return(fRmsTotalAccele);
}


int sfwGetItityLevel(void)
{
    return(wInityLv);
}
