#include    "./include/colorledapi.h"
#include    <stdio.h>
#include    <fcntl.h>

#define DEVICE_LED_BLUE     "/dev/led_blue"
#define DEVICE_LED_GREEN    "/dev/led_green"
#define DEVICE_LED_YELLOW   "/dev/led_yellow"
#define DEVICE_LED_RED      "/dev/led_red"

#define LED_BLUE            0
#define LED_GREEN           1
#define LED_YELLOW          2
#define LED_RED             3


int sfSetLedOutput(int wLedColor,char bValue)
{
    int wfdLeddev;
    int wRet;

    if((bValue == 0) || (bValue == 1))
    {
        switch(wLedColor)
        {
            case LED_BLUE:
                wfdLeddev = open(DEVICE_LED_BLUE, O_WRONLY);
                wRet = write(wfdLeddev, &bValue, 1);
                close(wfdLeddev);
                if(wRet != 1)
                {
                    printf("Write LED_BLUE fail\n");
                    return(-1);
                }
                break;
            case LED_GREEN:
                wfdLeddev = open(DEVICE_LED_GREEN, O_WRONLY);
                wRet = write(wfdLeddev, &bValue, 1);
                close(wfdLeddev);
                if(wRet != 1)
                {
                    printf("Write LED_GREEN fail\n");
                    return(-1);
                }
                break;
            case LED_YELLOW:
                wfdLeddev = open(DEVICE_LED_YELLOW, O_WRONLY);
                wRet = write(wfdLeddev, &bValue, 1);
                close(wfdLeddev);
                if(wRet != 1)
                {
                    printf("Write LED_YELLOW fail\n");
                    return(-1);
                }
                break;
            case LED_RED:
                wfdLeddev = open(DEVICE_LED_RED, O_WRONLY);
                wRet = write(wfdLeddev, &bValue, 1);
                close(wfdLeddev);
                if(wRet != 1)
                {
                    printf("Write LED_RED fail\n");
                    return(-1);
                }
                break;
            default:
                printf("LED Channel Error\n");
                return(-1);
                break;
        }
    }
    else
    {
        printf("LED value Error\n");
        return(-1);
    }
    return(0);
}
