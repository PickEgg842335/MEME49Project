#include "./include/app.h"

int main(void)
{
    int wDatax, wDatay ,wDataz;
    sfinitaladxl485();
    sfinitialws2812Led();

    while(1)
    {
        sfCalerthItity();
        printf("ID = %x\n", sfubGetadxl485DeviceId());
        printf("RMSACC = %f\n",sffGetRmsTotalAcccle());
        printf("Lv = %d\n",sfwGetItityLevel());
        sfWarmingLedDisplay(sfwGetItityLevel());
        sleep(1);
    }
    return 0;
}