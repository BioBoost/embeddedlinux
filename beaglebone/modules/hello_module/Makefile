# Makefile for hello module
ifneq ($(KERNELRELEASE),)
obj-m := hello.o
else

# Hardcoded paths (need refactoring)
KDIR := /home/bioboost/beaglebone/eewiki/linux-dev/KERNEL
CC := /home/bioboost/beaglebone/eewiki/gcc-linaro-arm-linux-gnueabihf-4.8-2013.07-1_linux/bin/arm-linux-gnueabihf-

all:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CC) -C $(KDIR) M=$$PWD modules

clean:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CC) -C $(KDIR) M=$$PWD clean

endif
