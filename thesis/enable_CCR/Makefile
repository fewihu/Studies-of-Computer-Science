obj-m	:= pmu_el0_cycle_counter.o
KDIR	:= /usr/src/linux-headers-5.15.32-v8+
PWD	:= $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
