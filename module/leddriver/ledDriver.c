#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/gpio.h> 
#include <linux/cdev.h>
#include <linux/interrupt.h>

#include "./include/gpiopin.h"

//LED is connected to this GPIO
#define MAX_LED_DEVICE      4
#define PIN_LED_BLUE        Rpi4_GPIO5
#define PIN_LED_GREEN       Rpi4_GPIO22
#define PIN_LED_YELLO       Rpi4_GPIO27
#define PIN_LED_RED         Rpi4_GPIO17
#define DEVICE_MAJOR 233
#define DEVICE_MAJOR_NAME   "led_4color"

const unsigned int uwLedPin[MAX_LED_DEVICE] = {
    PIN_LED_BLUE,
    PIN_LED_GREEN,
    PIN_LED_YELLO,
    PIN_LED_RED,
};

const unsigned char* ubDevMinorName[MAX_LED_DEVICE] = {
    "led_blue",
    "led_green",
    "led_yellow",
    "led_red",
};

static char bWriteData[MAX_LED_DEVICE];

static int led_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int led_release(struct inode *inode, struct file *file)
{
    return 0;
}


static ssize_t led_write(struct file *file, const char* buffer, size_t size, loff_t *off)
{
    int dev_minor = iminor(file->f_path.dentry->d_inode);
    unsigned char ubOutputLevel = 0;

    if(size == 1)
    {
        get_user(bWriteData[dev_minor], buffer++);
        ubOutputLevel = bWriteData[dev_minor] & 0x01;
        gpio_direction_output(uwLedPin[dev_minor], ubOutputLevel);
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
    .open = led_open,
    .write = led_write,
    .release = led_release,
};
//static dev_t  devno = 0;
static struct cdev mycdev;
static struct class *led_class;
static struct device *led_device[MAX_LED_DEVICE];


static int __init ledDevice_init(void)
{
    /*
     * Register device
    */
    int ret;

    ret = register_chrdev_region(MKDEV(DEVICE_MAJOR,0), MAX_LED_DEVICE, DEVICE_MAJOR_NAME);
    if (ret < 0) {
        printk(KERN_INFO "%s: registering device %s with major %d failed with %d\n",
            __func__, DEVICE_MAJOR_NAME, DEVICE_MAJOR, DEVICE_MAJOR );
        return(ret);
    }
    cdev_init(&mycdev, &strled_dev_fops);
    mycdev.owner = THIS_MODULE;

    ret = cdev_add(&mycdev, MKDEV(DEVICE_MAJOR,0), MAX_LED_DEVICE);
    if(ret)
    {
        printk("Error adding cdev\n");
    }

    led_class = class_create(DEVICE_MAJOR_NAME);
    if (IS_ERR(led_class))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), MAX_LED_DEVICE);
        return PTR_ERR(led_class);
    }

    for(int i = 0; i < MAX_LED_DEVICE; i++)
    {
        led_device[i] = device_create(led_class, NULL, MKDEV(DEVICE_MAJOR, i), NULL, ubDevMinorName[i]);
        if (IS_ERR(led_device))
        {
            printk(KERN_WARNING "Can't make node %d, %d\n", DEVICE_MAJOR, i);
            for(int j = 0; j < i; j++)
            {
                device_del(led_device[j]);
            }
            class_destroy(led_class);
            cdev_del(&mycdev);
            unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), MAX_LED_DEVICE);
            return PTR_ERR(led_device[i]);
        }
    }

    // Reserve gpios
    for(int i = 0; i < MAX_LED_DEVICE; i++)
    {
        ret = gpio_request(uwLedPin[i], ubDevMinorName[i]);
        if(ret < 0)
        {
            printk( KERN_INFO "%s: %s unable to get TRIG gpio\n", ubDevMinorName[i], __func__ );
            for(int j = 0; j < i; j++)
            {
                gpio_direction_input(uwLedPin[j]);
                gpio_free(uwLedPin[j]);
            }
            for(int j = 0; j < MAX_LED_DEVICE; j++)
            {
                device_del(led_device[j]);
            }
            class_destroy(led_class);
            cdev_del(&mycdev);
            unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), MAX_LED_DEVICE);
            ret = -EBUSY;
            return(ret);
        }
    }

    // Set gpios directions
    for(int i = 0; i < MAX_LED_DEVICE; i++)
    {
        ret = gpio_direction_output(uwLedPin[i], 0);
        if(ret < 0 )
        {
            printk( KERN_INFO "%s: %s unable to set TRIG gpio as output\n", ubDevMinorName[i], __func__ );
            for(int j = 0; j < MAX_LED_DEVICE; j++)
            {
                gpio_direction_input(uwLedPin[j]);
                gpio_free(uwLedPin[j]);
                device_del(led_device[j]);
            }
            class_destroy(led_class);
            cdev_del(&mycdev);
            unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), MAX_LED_DEVICE);
            ret = -EBUSY;
            return(ret);
        }
    }

    return 0;
}


static void __exit ledDevice_exit(void)
{
    for(int i = 0; i < MAX_LED_DEVICE; i++)
    {
        gpio_direction_input(uwLedPin[i]);
        gpio_free(uwLedPin[i]);
        device_del(led_device[i]);
    }
    class_destroy(led_class);
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), MAX_LED_DEVICE);
}

module_init(ledDevice_init);
module_exit(ledDevice_exit);
MODULE_LICENSE("GPL");