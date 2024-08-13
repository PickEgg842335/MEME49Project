#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "./include/gpiopin.h"

//LED is connected to this GPIO
#define PIN Rpi4_GPIO4
#define DEVICE_MAJOR 232
#define DEVICE_NAME "dht11"

unsigned char   ubResultBuf[5];
unsigned int    uwResultCheckSum = 0;
unsigned char   ubcheck_flag;


int data_in(void)
{
    gpio_direction_input(PIN);
    return gpio_get_value(PIN);
}


void gpio_out(int value)   //set gpio is output
{
    gpio_direction_output(PIN,value);
}


void dht11_read_data(void)
{
    int wTimeout = 0;
    unsigned char ubflag = 0;
    unsigned char ubBitdataTemp = 0;

    gpio_out(0);
    mdelay(30);
    gpio_out(1);
    udelay(40);    
    if(data_in() == 0)
    {
        wTimeout = 0;
        while(!gpio_get_value(PIN))
        {
            udelay(5);
            wTimeout++;
            if(wTimeout > 20)
            {
                printk("error data!\n");
                break;
            }
        }
        wTimeout = 0;
        while(gpio_get_value(PIN))
        {
            udelay(5);
            wTimeout++;
            if(wTimeout > 20)
            {
                printk("error data!\n");
                break;
            }
        }
    }
    for(int i = 0; i < 5; i++)
    {
        for(int wbitindex = 0; wbitindex < 8; wbitindex++)
        {              
            wTimeout = 0;
            while( !gpio_get_value(PIN) )
            {
                udelay(10);
                wTimeout++;
                if(wTimeout > 40)
                {
                    break;
                }
            }
            ubflag = 0x0;
            udelay(28);
            if(gpio_get_value(PIN))
            {
                ubflag = 0x01;
            }
            wTimeout = 0;
            while(gpio_get_value(PIN))
            {
                udelay(10);
                wTimeout++;
                if(wTimeout > 60)
                {
                    break;
                }
            }
            ubBitdataTemp <<= 1;
            ubBitdataTemp |= ubflag;
        }
        ubResultBuf[i] = ubBitdataTemp;
    }
    uwResultCheckSum = ubResultBuf[0] + ubResultBuf[1] + ubResultBuf[2] + ubResultBuf[3];
    uwResultCheckSum &= 0x00FF;
    if((unsigned int)uwResultCheckSum == ubResultBuf[4])
    {
        ubcheck_flag = 0xff;
        printk("dht11 check pass\n");
        printk("humidity=[%d],temp=[%d]\n", ubResultBuf[0], ubResultBuf[2]);
    }
    else
    {
        ubcheck_flag = 0x00;
        printk("dht11 check fail\n");           
    }                   
}


static int dht11_open(struct inode *inode, struct file *file)
{
    printk("open dht11 in kernel\n");
    return 0;
}


static int dht11_release(struct inode *inode, struct file *file)
{
    printk("dht11 release\n");
    return 0;
}


static ssize_t dht11_read(struct file *file, char* buffer, size_t size, loff_t *off)
{
    int wRet;
    local_irq_disable();
    printk("read dht11\n");
    dht11_read_data();
    local_irq_enable();
    if(ubcheck_flag == 0xff)
    {
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
    else
    {
        return -EAGAIN;
    }
}


static struct file_operations strdht11_dev_fops={
    .owner = THIS_MODULE,
    .open = dht11_open,
    .read = dht11_read,
    .release = dht11_release,
};
static struct class *dht_class;
static struct device *dht_device;


static int __init dht11Device_init(void)
{
    /*
     * Register device
    */
    int wRet;

    wRet = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &strdht11_dev_fops);
    if (wRet < 0) {
        printk(KERN_INFO "%s: registering device %s with major %d failed with %d\n",
            __func__, DEVICE_NAME, DEVICE_MAJOR, DEVICE_MAJOR );
        return(wRet);
    }
    printk("DHT11 driver register success!\n");

    dht_class = class_create(DEVICE_NAME);
    if (IS_ERR(dht_class))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
        return PTR_ERR(dht_class);
    }

    dht_device = device_create(dht_class, NULL, MKDEV(DEVICE_MAJOR, 0), NULL, DEVICE_NAME);
    if (IS_ERR(dht_device))
    {
        printk(KERN_WARNING "Can't make node %d\n", DEVICE_MAJOR);
        unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
        class_destroy(dht_class);
        return PTR_ERR(dht_device);
    }

    printk("DHT11 driver make node success!\n");

    // Reserve gpios
    if( gpio_request( PIN, DEVICE_NAME ) < 0 )	// request pin 2
    {
        printk( KERN_INFO "%s: %s unable to get TRIG gpio\n", DEVICE_NAME, __func__ );
        unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
        device_del(dht_device);
        class_destroy(dht_class);
        wRet = -EBUSY;
        return(wRet);
    }

    // Set gpios directions
    if( gpio_direction_output( PIN, 1) < 0 )	// Set pin 2 as output with default value 0
    {
        printk( KERN_INFO "%s: %s unable to set TRIG gpio as output\n", DEVICE_NAME, __func__ );
        unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
        device_del(dht_device);
        class_destroy(dht_class);
        wRet = -EBUSY;
        return(wRet);
    }

    return 0;
}


static void __exit dht11Device_exit(void)
{
    gpio_direction_input(PIN);
    gpio_free(PIN);
    unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
    device_del(dht_device);
    class_destroy(dht_class);
}

module_init(dht11Device_init);
module_exit(dht11Device_exit);
MODULE_LICENSE("GPL");