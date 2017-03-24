obj-m += hello.o

CFLAGS_hello.o := -DDEBUG

CROSS=/home/gaurav/Documents/Galeilo/buildroot-2016.11.2/output/host/usr/bin/i586-buildroot-linux-uclibc-
ifndef KSRC
KSRC = /lib/modules/$(shell uname -r)/build
endif
all:
	make -C $(KSRC) M=$(PWD) CROSS_COMPILE=$(CROSS) modules
clean:
	make -C $(KSRC) M=$(PWD) clean
