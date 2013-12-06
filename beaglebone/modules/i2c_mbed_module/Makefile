# Makefile for i2c mbed module
ifneq ($(KERNELRELEASE),)
obj-m := i2c_mbed_module.o
else

# Hardcoded paths (need refactoring)
KDIR := /home/bioboost/beaglebone/eewiki/linux-dev/KERNEL
CC := /home/bioboost/beaglebone/eewiki/gcc-linaro-arm-linux-gnueabihf-4.8-2013.07-1_linux/bin/arm-linux-gnueabihf-

all:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CC) -C $(KDIR) M=$$PWD modules
	$(CC)gcc userapp.c -o userapp

clean:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CC) -C $(KDIR) M=$$PWD clean
	rm -f *.o userapp

endif
