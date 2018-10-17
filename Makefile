# This is a basic makefile for the keyboard driver
# 
#  This make file compiles a C file into a kernel
#  module.
 
#  Plan to add insmod command to
#  add to the running Kernel tree and.
#
#  clean will remove all the resources and take the
#  module off of the tree


ifneq ($(KERNELRELEASE),)
	obj-m := keyboarddrvr.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:	
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order

endif

