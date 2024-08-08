#include "./include/erthItity.h"

int     wAdcAcceleX = 0;
int     wAdcAcceleY = 0;
int     wAdcAcceleZ = 0;
float   fRealTotalAccele = 0;
float   fRmsTotalAccele = 0;

void sfCalerthItity(void);
void sfGetDataXYZ(void);


void sfCalerthItity(void)
{
    float fTotalAcceleArray[100];
    float fTemp = 0;

    for(int i = 0; i < 100; i++)
    {
        wAdcAcceleX = sfwGetadxl485DataX();
        wAdcAcceleY = sfwGetadxl485DataY();
        wAdcAcceleZ = sfwGetadxl485DataZ();
        fTotalAcceleArray[i] = sqrtf(wAdcAcceleX * wAdcAcceleX + wAdcAcceleY * wAdcAcceleY + wAdcAcceleZ * wAdcAcceleZ) / 256.0;
        usleep(1000);
    }

    for(int i = 0; i < 100; i++)
    {
        fTemp += fTotalAcceleArray[i] * fTotalAcceleArray[i];
    }
    fRmsTotalAccele = fTemp / 100;
}


float sffGetRmsTotalAcccle(void)
{
    return(fRmsTotalAccele);
}
