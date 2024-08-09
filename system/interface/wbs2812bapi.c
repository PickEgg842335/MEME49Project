#include    "./include/wbs2812bapi.h"

#define DEVICE_MAJOR    234
#define IOCTL_SET_WBSQTY        _IOW(DEVICE_MAJOR,0, int)
#define IOCTL_SET_WBSDATA       _IOW(DEVICE_MAJOR,1, unsigned char *)
#define IOCTL_SET_WBSDISPLAY    _IO(DEVICE_MAJOR,2)
#define DEVICE_NAME             "/dev/wbs2812b"

int wfdwbs2812bdev;


void    sfOpenWbs2812bLedEnable(int wTemp)
{
    switch(wTemp) 
    {
        case true:
            wfdwbs2812bdev = open(DEVICE_NAME, O_WRONLY);
            break;
        case false:
            close(wfdwbs2812bdev);
            break;
        default:
            break;
    }
}


int     sfSetWbs2812bQty(unsigned int uwQty)
{
    int wRet;

    wRet = ioctl(wfdwbs2812bdev, IOCTL_SET_WBSQTY, uwQty);
    if(wRet < 0)
    {
        printf("wbs2812b driver: WBSQTY set fail.\n");
        return(wRet);
    }
    return 0;
}


int     sfSetWbs2812bRGBData(unsigned int uwIndex, unsigned char ubRGBData[3])
{
    int wRet;
    unsigned char ubData[5];
    ubData[0] = (unsigned char)(uwIndex >> 8);
    ubData[1] = (unsigned char)(uwIndex & 0x00FF);
    ubData[2] = ubRGBData[0];
    ubData[3] = ubRGBData[1];
    ubData[4] = ubRGBData[2];
    wRet = ioctl(wfdwbs2812bdev, IOCTL_SET_WBSDATA, ubData);
    if(wRet < 0)
    {
        printf("wbs2812b driver: WBSDATA Index is bigger than Real Qty.\n");
        return(wRet);
    }
    return 0;
}


int     sfSetWbs2812bStartDisplay(void)
{
    int wRet;

    wRet = ioctl(wfdwbs2812bdev, IOCTL_SET_WBSDISPLAY);
    if(wRet < 0)
    {
        printf("wbs2812b driver: WBSDISPLAY is fail.\n");
        return(wRet);
    }
    return 0;
}