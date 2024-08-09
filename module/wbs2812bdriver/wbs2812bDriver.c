#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/pwm.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <uapi/asm-generic/ioctl.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>

#include "./include/gpiopin.h"

#define GPIO_BASE 0xFE200000
#define PWM_BASE  0xFE20C000
#define CLOCK_BASE 0xFE101000

#define GPFSEL1     0x04
#define GPSET0      0x1C
#define GPCLR0      0x28
#define PWM_CTL     0x00
#define PWM_STA     0x04
#define PWM_RNG1    0x10
#define PWM_DAT1    0x14
#define PWM_FIFO1   0x18
#define PWMCLK_CNTL 0xA0
#define PWMCLK_DIV  0xA4

#define PWMDIV      2
#define PWMRANGE    34
#define PWMIdle     0
#define PWMOUTLOW   11
#define PWMOUTHIGH  23
#define PWMHOLD     34

#define GPIO_ALT0 4

#define PIN     Rpi4_GPIO12
#define DEVICE_MAJOR    234
#define DEVICE_NAME     "wbs2812b"
#define MAXWBS2812BQTY  6

static void __iomem *gpio_base;
static void __iomem *pwm_base;
static void __iomem *clk_base;

// ioctl paramet
#define IOCTL_SET_WBSQTY        _IOW(DEVICE_MAJOR,0, int)
#define IOCTL_SET_WBSDATA       _IOW(DEVICE_MAJOR,1, unsigned char *)
#define IOCTL_SET_WBSDISPLAY    _IO(DEVICE_MAJOR,2)

struct wbs2812b
{
    struct cdev     cdev;
    struct class    *class;
    struct device   *dev;

    unsigned int    uwRealQty;
    unsigned char   ubDisplayData[MAXWBS2812BQTY][3];
    unsigned char   ubBusyFlag;
};

struct wbs2812b mywbs2812b;


static int getFIFOEmpty(void)
{
    pwm_base = ioremap(PWM_BASE, 0x28);
    return ((readl(gpio_base + PWM_STA) & 0x02) && true);
}


static void setStartPwmFIFO(void)
{
    pwm_base = ioremap(PWM_BASE, 0x28);
    writel(0x81 | 0x20, pwm_base + PWM_CTL);
}


static void setStopPwmFIFO(void)
{
    pwm_base = ioremap(PWM_BASE, 0x28);
    writel(0x81, pwm_base + PWM_CTL);
}


static void setPwmFIFO(unsigned int uwPwmValue)
{
    pwm_base = ioremap(PWM_BASE, 0x28);
    writel(uwPwmValue, pwm_base + PWM_FIFO1);
}


static void setPwm0Output(unsigned int uwPwmValue)
{
    pwm_base = ioremap(PWM_BASE, 0x28);
    writel(uwPwmValue, pwm_base + PWM_DAT1);
}


static void setPwm0setup(void)
{
    pwm_base = ioremap(PWM_BASE, 0x28);
    clk_base = ioremap(CLOCK_BASE, 0xA8);
    // Stop PWM clock and waiting for busy flag doesn't work, so kill clock
    writel(0x5A000000 | (1 << 5), clk_base + PWMCLK_CNTL);
    udelay(10);

    // Set PWM clock divider to 32 (60MHz/8 = 600kHz)
    writel(0x5A000000 | (PWMDIV << 12), clk_base + PWMCLK_DIV);
    udelay(10);

    // Start PWM clock
    writel(0x5A000011, clk_base + PWMCLK_CNTL);
    udelay(10);

    // Set PWM range register
    writel(PWMRANGE, pwm_base + PWM_RNG1);
    udelay(10);

    // Set PWM data register
    writel(PWMIdle, pwm_base + PWM_DAT1);
    udelay(10);

    // Enable PWM channel
    writel(0x81, pwm_base + PWM_CTL);
}


static void setGpio12toPwm0_0(void)
{
    gpio_base = ioremap(GPIO_BASE, 0xF4);
    writel((readl(gpio_base + GPFSEL1) & ~(7 << 6)) | (GPIO_ALT0 << 6), gpio_base + GPFSEL1);
}


static int wbs2812bBitOut(unsigned char ubBit)
{
    switch(ubBit)
    {
        case 0:
            setPwmFIFO(PWMOUTLOW);
        break;
        case 1:
            setPwmFIFO(PWMOUTHIGH);
        break;
        default:
            return -1;
        break;
    }
    return 0;
}


static int wbs2812b_open(struct inode *inode, struct file *file)
{
    mywbs2812b.uwRealQty = MAXWBS2812BQTY;
    mywbs2812b.ubBusyFlag = false;
    memset(mywbs2812b.ubDisplayData, 0, sizeof(mywbs2812b.ubDisplayData));
    printk("open wbs2812b driver in kernel\n");
    return 0;
}


static int wbs2812b_release(struct inode *inode, struct file *file)
{
    mywbs2812b.uwRealQty = MAXWBS2812BQTY;
    mywbs2812b.ubBusyFlag = false;
    memset(mywbs2812b.ubDisplayData, 0, sizeof(mywbs2812b.ubDisplayData));
    printk("wbs2812b release\n");
    return 0;
}


static ssize_t wbs2812b_write(struct file *file, const char* buffer, size_t size, loff_t *off)
{
    return -1;
}


static long wbs2812b_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret;
    int wLengthTemp;
    int wDataTemp;
    unsigned int  uwDataTemp;
    unsigned char ubDataTemp[5];
    unsigned char ubTemp;

    switch(cmd)
    {
        case IOCTL_SET_WBSQTY:
            wDataTemp = (int)arg;
            if(((wDataTemp < 0) || (wDataTemp > MAXWBS2812BQTY))
                || (mywbs2812b.ubBusyFlag == true))
            {
                printk("wbs2812b driver: WBSQTY set fail.");
                return -1;
            }
            mywbs2812b.uwRealQty = wDataTemp;
            printk("wbs2812b driver: WBSQTY set success.");
            printk("WBSQTY = %d",mywbs2812b.uwRealQty);
            break;
        case IOCTL_SET_WBSDATA:
            wLengthTemp = sizeof((char *)arg);
            if((wLengthTemp < 5) || (mywbs2812b.ubBusyFlag == true))
            {
                printk("wbs2812b driver: WBSDATA set fail.");
                return -1;
            }

            for(int i = 0; i < 5; i++)
            {
                get_user(ubDataTemp[i], (char *)(arg + i));
            }
            uwDataTemp = ((unsigned int)ubDataTemp[0] << 8) + (unsigned int)ubDataTemp[1];
            if((uwDataTemp > mywbs2812b.uwRealQty) || (uwDataTemp == 0))
            {
                printk("wbs2812b driver: WBSDATA Index is bigger than Real Qty.");
                return -1;
            }
            mywbs2812b.ubDisplayData[uwDataTemp - 1][0] = ubDataTemp[3];
            mywbs2812b.ubDisplayData[uwDataTemp - 1][1] = ubDataTemp[2];
            mywbs2812b.ubDisplayData[uwDataTemp - 1][2] = ubDataTemp[4];
            printk("wbs2812b driver: WBSDATA set success.");
            printk("WBSDATA2 = %d %d %d %d", (uwDataTemp - 1), (int)mywbs2812b.ubDisplayData[uwDataTemp - 1][0], (int)mywbs2812b.ubDisplayData[uwDataTemp - 1][1], (int)mywbs2812b.ubDisplayData[uwDataTemp - 1][2]);
            break;
        case IOCTL_SET_WBSDISPLAY:
            mywbs2812b.ubBusyFlag = true;
            setPwm0Output(PWMIdle);
            setStopPwmFIFO();
            for(int i = 0; i < 2; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    for(int k = 8; k > 0; k--)
                    {
                        ubTemp = ((mywbs2812b.ubDisplayData[i][j]) & (1 << (k - 1))) && true;
                        ret = wbs2812bBitOut(ubTemp);
                    }
                }
            }
            setPwmFIFO(PWMIdle);
            setPwm0Output(PWMIdle);
            setStartPwmFIFO();
            mdelay(1);
            mywbs2812b.ubBusyFlag = false;
            break;
    }
    return 0;
}


static struct file_operations strWbs2812b_dev_fops={
    .owner = THIS_MODULE,
    .open = wbs2812b_open,
    .write = wbs2812b_write,
    .unlocked_ioctl = wbs2812b_ioctl,
    .release = wbs2812b_release,
};

static int __init wbs2812bDevice_init(void)
{
    /*
     * Register device
    */
    int ret;
    gpio_base = ioremap(GPIO_BASE, 0xF4);
    pwm_base = ioremap(PWM_BASE, 0x28);
    clk_base = ioremap(CLOCK_BASE, 0xA8);

    mywbs2812b.ubBusyFlag = false;
    ret = register_chrdev_region(MKDEV(DEVICE_MAJOR,0), 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_INFO "%s: registering device %s with major %d failed with %d\n",
            __func__, DEVICE_NAME, DEVICE_MAJOR, DEVICE_MAJOR );
        return(ret);
    }
    printk("WBS2812b-LED driver register success!\n");
    cdev_init(&mywbs2812b.cdev, &strWbs2812b_dev_fops);
    mywbs2812b.cdev.owner = THIS_MODULE;

    ret = cdev_add(&mywbs2812b.cdev, MKDEV(DEVICE_MAJOR,0), 1);
    if(ret)
    {
        printk("Error adding cdev\n");
    }

    mywbs2812b.class = class_create(DEVICE_NAME);
    if (IS_ERR(mywbs2812b.class))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        cdev_del(&mywbs2812b.cdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return PTR_ERR(mywbs2812b.class);
    }

    mywbs2812b.dev = device_create(mywbs2812b.class, NULL, MKDEV(DEVICE_MAJOR, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mywbs2812b.dev))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        class_destroy(mywbs2812b.class);
        cdev_del(&mywbs2812b.cdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return PTR_ERR(mywbs2812b.dev);
    }

    ret = gpio_request(PIN, DEVICE_NAME);
    if(ret < 0)
    {
        printk( KERN_INFO "%s: %s unable to get TRIG gpio\n", DEVICE_NAME, __func__ );
        device_del(mywbs2812b.dev);
        class_destroy(mywbs2812b.class);
        cdev_del(&mywbs2812b.cdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        ret = -EBUSY;
        return(ret);
    }
    printk("GPIO12 requests success \n");

    setGpio12toPwm0_0();
    printk("GPIO12 is set ALT0 function PWM0_0\n");

    setPwm0setup();
    printk(KERN_INFO "GPIO12 is set to PWM mode\n");
    setPwmFIFO(PWMIdle);
    setStartPwmFIFO();
    return 0;
}


static void __exit wbs2812bDevice_exit(void)
{
    int ret;

    mywbs2812b.ubBusyFlag = true;
    setPwm0Output(PWMIdle);
    setStopPwmFIFO();
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 8; k > 0; k--)
            {
                ret = wbs2812bBitOut(0);
            }
        }
    }
    setPwmFIFO(PWMIdle);
    setStartPwmFIFO();
    mdelay(10);
    mywbs2812b.ubBusyFlag = false;
    iounmap(gpio_base);
    iounmap(pwm_base);
    iounmap(clk_base);
    gpio_direction_input(PIN);
    gpio_free(PIN);
    device_del(mywbs2812b.dev);
    class_destroy(mywbs2812b.class);
    cdev_del(&mywbs2812b.cdev);
    unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
    printk(KERN_INFO "GPIO12 disabled\n");
}

module_init(wbs2812bDevice_init);
module_exit(wbs2812bDevice_exit);
MODULE_LICENSE("GPL");