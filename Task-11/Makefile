CC=/home/gaurav/Documents/buildroot-2017.02.5/output/host/usr/bin/i586-linux-gcc
KSRC=/home/gaurav/Documents/buildroot-2017.02.5/output/build/linux-0feded4f96ddca9816c6c1b8a4780cb4065747b9/

obj-m += adc1x8s102.o

CFLAGS_debug_fs.o := -DDEBUG

all:
	make -C $(KSRC) M=$(PWD) modules
clean:
	make -C $(KSRC) M=$(PWD) clean
