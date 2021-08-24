obj-m += sysctl_var.o

KERNELDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

insert:
	insmod sysctl_var.ko

remove:
	rmmod sysctl_var.ko
