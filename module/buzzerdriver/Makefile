#XP kernel pathus/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
4412_KERNEL_DIR = /lib/modules/$(shell uname -r)/build
##/home/joeko/android-kernel-dma4412u
ifneq ($(KERNELRELEASE),)
	obj-m := buzzerdriver.o
else
	KERNELDIR ?= $(4412_KERNEL_DIR)
	
	PWD := $(shell pwd)
	
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	
clean:
	-rm -f *.ko *.o *.bak *.mod.* *.mod *.order *.symvers .*
endif 
