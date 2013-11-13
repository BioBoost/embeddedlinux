# Makefile for hello module
ifneq ($(KERNELRELEASE),)
obj-m := charmod.o
else

# Hardcoded paths (need refactoring)
KDIR = /usr/src/linux-headers-3.2.0-29-generic-pae

all:
	$(MAKE) -C $(KDIR) M=$$PWD modules
	gcc userapp.c -o userapp

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	rm -f *.o userapp

endif
