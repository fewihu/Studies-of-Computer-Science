obj-m += lkm_emptyM.o

all:
	make -C /usr/src/linux-headers-5.15.32-v8+ M=$(PWD) modules
clean:
	make -C /usr/src/linux-headers-5.15.32-v8+ M=$(PWD) clean
