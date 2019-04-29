CFLAGS=-g
ifneq ($(KERNELRELEASE),)

    obj-m := hackme.o

else

    #KERN_DIR ?= /usr/src/linux-headers-$(shell uname -r)/
    KERN_DIR ?= ./build/linux
    PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules

endif


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

# vim:ft=make
#
