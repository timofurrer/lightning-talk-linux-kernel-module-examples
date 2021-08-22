obj-m += hello_world.o

KERNELDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

insert:
	insmod hello_world.ko

remove:
	rmmod hello_world.ko
