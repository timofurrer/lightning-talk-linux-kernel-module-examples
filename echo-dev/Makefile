obj-m += echo_dev.o

KERNELDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

insert:
	insmod echo_dev.ko

remove:
	rmmod echo_dev.ko
