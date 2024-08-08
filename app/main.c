#include "./include/app.h"

int main(void)
{
    int wDatax, wDatay ,wDataz;
    sfinitaladxl485();

    while(1)
    {
        sfCalerthItity();
        printf("ID = %x\n", sfubGetadxl485DeviceId());
        printf("RMSACC = %f\n",sffGetRmsTotalAcccle());
        sleep(1);
    }
    return 0;
}