ifneq ($(KERNELRELEASE),)
obj-m:=load5s.o
else
KERNELVER?=$(shell uname -r)
ifeq ("$(origin KVER)", "command line")
  KERNELVER:=$(KVER)
endif
KERNELDIR:=/usr/src/kernels/$(KERNELVER)/
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif
