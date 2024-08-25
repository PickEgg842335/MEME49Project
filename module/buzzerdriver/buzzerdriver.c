#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/gpio.h> 
#include <linux/cdev.h>
#include <linux/interrupt.h>

#include "./include/gpiopin.h"

//Buzzer is connected to this GPIO
#define PIN_BUZZER          Rpi4_GPIO25
#define DEVICE_MAJOR        235
#define DEVICE_NAME         "buzzer"

static char bWriteData;

static int buzzer_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int buzzer_release(struct inode *inode, struct file *file)
{
    return 0;
}


static ssize_t buzzer_write(struct file *file, const char* buffer, size_t size, loff_t *off)
{
    unsigned char ubOutputLevel = 0;

    if(size == 1)
    {
        get_user(bWriteData, buffer++);
        ubOutputLevel = bWriteData & 0x01;
        gpio_direction_output(PIN_BUZZER, ubOutputLevel);
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
    .open = buzzer_open,
    .write = buzzer_write,
    .release = buzzer_release,
};

static struct cdev mycdev;
static struct class *buzzer_class;
static struct device *buzzer_device;


static int __init buzzerDevice_init(void)
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
    cdev_init(&mycdev, &strled_dev_fops);
    mycdev.owner = THIS_MODULE;

    ret = cdev_add(&mycdev, MKDEV(DEVICE_MAJOR,0), 1);
    if(ret)
    {
        printk("Error adding cdev\n");
    }

    buzzer_class = class_create(DEVICE_NAME);
    if (IS_ERR(buzzer_class))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return PTR_ERR(buzzer_class);
    }

    buzzer_device = device_create(buzzer_class, NULL, MKDEV(DEVICE_MAJOR, 0), NULL, DEVICE_NAME);
    if (IS_ERR(buzzer_device))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        device_del(buzzer_device);
        class_destroy(buzzer_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        return PTR_ERR(buzzer_device);
    }

    // Reserve gpios
    ret = gpio_request(PIN_BUZZER, DEVICE_NAME);
    if(ret < 0)
    {
        printk( KERN_INFO "%s: %s unable to get TRIG gpio\n", DEVICE_NAME, __func__ );
        gpio_direction_input(PIN_BUZZER);
        gpio_free(PIN_BUZZER);
        device_del(buzzer_device);
        class_destroy(buzzer_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        ret = -EBUSY;
        return(ret);
    }

    // Set gpios directions
    ret = gpio_direction_output(PIN_BUZZER, 0);
    if(ret < 0 )
    {
        printk( KERN_INFO "%s: %s unable to set TRIG gpio as output\n", DEVICE_NAME, __func__ );
        gpio_direction_input(PIN_BUZZER);
        gpio_free(PIN_BUZZER);
        device_del(buzzer_device);
        class_destroy(buzzer_class);
        cdev_del(&mycdev);
        unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
        ret = -EBUSY;
        return(ret);
    }

    return 0;
}


static void __exit buzzerDevice_exit(void)
{
    gpio_direction_input(PIN_BUZZER);
    gpio_free(PIN_BUZZER);
    device_del(buzzer_device);
    class_destroy(buzzer_class);
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(DEVICE_MAJOR, 0), 1);
}

module_init(buzzerDevice_init);
module_exit(buzzerDevice_exit);
MODULE_LICENSE("GPL");