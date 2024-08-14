#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/gpio.h> 
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#include "./include/gpiopin.h"

//LED is connected to this GPIO
#define PIN_HR04_TRIG       Rpi4_GPIO23
#define PIN_HR04_ECHO       Rpi4_GPIO24
#define DEVICE_MAJOR        231
#define DEVICE_NAME         "hchr04"
#define PIN_TRIG_NAME       "hr04_trig"
#define PIN_ECHO_NAME       "hr04_echo"

static char bHR04WriteData;
static char bHR04BusyFlag = false;
static int  echo_irq;
static ktime_t start_time, end_time;
static int  distance;
static unsigned char   ubResultBuf[9];


static void hchr04_trigger_measurement(void)
{
    gpio_set_value(PIN_HR04_TRIG, 0);
    udelay(2);
    gpio_set_value(PIN_HR04_TRIG, 1);
    udelay(10);
    gpio_set_value(PIN_HR04_TRIG, 0);
}


static int hchr04_open(struct inode *inode, struct file *file)
{
    printk("open in kernel\n");
    bHR04BusyFlag = false;
    return 0;
}


static int hchr04_release(struct inode *inode, struct file *file)
{
    printk("HC-HR04 release\n");
    bHR04BusyFlag = false;
    return 0;
}

static ssize_t hchr04_read(struct file *file, char* buffer, size_t size, loff_t *off)
{
    int wRet;
    ktime_t ddtemp = ktime_to_us(ktime_sub(end_time, start_time));
    ubResultBuf[0] = bHR04BusyFlag;
    ubResultBuf[1] = (ddtemp >> 56) & 0xFF;
    ubResultBuf[2] = (ddtemp >> 48) & 0xFF;
    ubResultBuf[3] = (ddtemp >> 40) & 0xFF;
    ubResultBuf[4] = (ddtemp >> 32) & 0xFF;
    ubResultBuf[5] = (ddtemp >> 24) & 0xFF;
    ubResultBuf[6] = (ddtemp >> 16) & 0xFF;
    ubResultBuf[7] = (ddtemp >> 8) & 0xFF;
    ubResultBuf[8] = ddtemp & 0xFF;
    wRet = copy_to_user(buffer,ubResultBuf,sizeof(ubResultBuf));
    if(wRet < 0)
    {
        printk("copy to user err\n");
        return -EAGAIN;
    }
    else
    {
        return 0;
    }
}


static ssize_t hchr04_write(struct file *file, const char* buffer, size_t size, loff_t *off)
{
    unsigned char ubOutputLevel = 0;

    if(size == 1)
    {
        get_user(bHR04WriteData, buffer++);
        ubOutputLevel = bHR04WriteData & 0x01;
        bHR04BusyFlag = true;
        hchr04_trigger_measurement();
        return 1;
    }
    else
    {
        printk( KERN_INFO "Write Data error\n");
        return -1;
    }
}


static struct file_operations strled_dev_fops={
    .owner = THIS_MODULE,
    .open = hchr04_open,
    .read = hchr04_read,
    .write = hchr04_write,
    .release = hchr04_release,
};

static struct cdev mycdev;
static struct class *hchr04_class;
static struct device *hchr04_device;


static irqreturn_t echo_irq_handler(int irq, void *dev_id)
{
    if (gpio_get_value(PIN_HR04_ECHO))
    {
        start_time = ktime_get();
    }
    else
    {
        end_time = ktime_get();
        distance = ktime_to_us(ktime_sub(end_time, start_time)) * 343 / 2 / 10000;
        bHR04BusyFlag = false;
        printk(KERN_INFO "HR04: Distance = %d cm\n", distance);
    }

    return IRQ_HANDLED;
}


static int __init hchr04Device_init(void)
{
    /*
     * Register device
    */
    int ret;

    ret = register_chrdev_region(MKDEV(DEVICE_MAJOR,0), 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_INFO "%s: registering device %s with major %d failed with %d\n",
            __func__, DEVICE_NAME, DEVICE_MAJOR, DEVICE_MAJOR );
        return(ret);
    }
    printk("HC-HR04 driver register success!\n");
    cdev_init(&mycdev, &strled_dev_fops);
    mycdev.owner = THIS_MODULE;

    ret = cdev_add(&mycdev, MKDEV(DEVICE_MAJOR,0), 1);
    if(ret)
    {
        printk("Error adding cdev\n");
    }

    hchr04_class = class_create(DEVICE_NAME);
    if (IS_ERR(hchr04_class))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return PTR_ERR(hchr04_class);
    }

    hchr04_device = device_create(hchr04_class, NULL, MKDEV(DEVICE_MAJOR, 0), NULL, DEVICE_NAME);
    if (IS_ERR(hchr04_device))
    {
        printk(KERN_WARNING "Can't make node %d, %d\n", DEVICE_MAJOR, 0);
        class_destroy(hchr04_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return PTR_ERR(hchr04_device);
    }

    printk("HC-HR04 driver make node success!\n");

    // Reserve gpios
    ret = gpio_request(PIN_HR04_TRIG, PIN_TRIG_NAME);
    if(ret < 0)
    {
        printk( KERN_INFO "%s: %s unable to get TRIG gpio\n", DEVICE_NAME, __func__ );
        device_del(hchr04_device);
        class_destroy(hchr04_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        ret = -EBUSY;
        return(ret);
    }

    ret = gpio_request(PIN_HR04_ECHO, PIN_ECHO_NAME);
    if(ret < 0)
    {
        printk( KERN_INFO "%s: %s unable to get TRIG gpio\n", DEVICE_NAME, __func__ );
        gpio_direction_input(PIN_HR04_TRIG);
        gpio_free(PIN_HR04_TRIG);
        device_del(hchr04_device);
        class_destroy(hchr04_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        ret = -EBUSY;
        return(ret);
    }

    // Set gpios directions
    gpio_direction_output(PIN_HR04_TRIG, 0);
    gpio_direction_input(PIN_HR04_ECHO);

    echo_irq = gpio_to_irq(PIN_HR04_ECHO);
    if (echo_irq < 0)
    {
        printk(KERN_ERR "HR04: Failed to get IRQ for ECHO GPIO\n");
        gpio_direction_input(PIN_HR04_TRIG);
        gpio_free(PIN_HR04_TRIG);
        gpio_direction_input(PIN_HR04_ECHO);
        gpio_free(PIN_HR04_ECHO);
        device_del(hchr04_device);
        class_destroy(hchr04_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return(echo_irq);
    }

    ret = request_irq(echo_irq, echo_irq_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "hchr04_echo_irq", NULL);
    if(ret)
    {
        printk(KERN_ERR "HR04: Failed to request IRQ\n");
        free_irq(echo_irq, NULL);
        gpio_direction_input(PIN_HR04_TRIG);
        gpio_free(PIN_HR04_TRIG);
        gpio_direction_input(PIN_HR04_ECHO);
        gpio_free(PIN_HR04_ECHO);
        device_del(hchr04_device);
        class_destroy(hchr04_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return ret;
    }

    return 0;
}


static void __exit hchr04Device_exit(void)
{
    free_irq(echo_irq, NULL);
    gpio_direction_input(PIN_HR04_TRIG);
    gpio_free(PIN_HR04_TRIG);
    gpio_direction_input(PIN_HR04_ECHO);
    gpio_free(PIN_HR04_ECHO);
    device_del(hchr04_device);
    class_destroy(hchr04_class);
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
}

module_init(hchr04Device_init);
module_exit(hchr04Device_exit);
MODULE_LICENSE("GPL");