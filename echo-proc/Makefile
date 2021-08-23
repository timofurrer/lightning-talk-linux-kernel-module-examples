obj-m += echo_proc.o

KERNELDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

insert:
	insmod echo_proc.ko

remove:
	rmmod echo_proc.ko
