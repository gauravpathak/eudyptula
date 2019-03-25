obj-m += list.o

CFLAGS_list.o := -DDEBUG

ifndef KSRC
KSRC = /lib/modules/$(shell uname -r)/build
CUR_DIR = $(shell pwd)
else
CCDIR = /opt/toolchains/gcc-linaro-6.2.1-2016.11-i686_aarch64-linux-gnu/bin/
CUR_DIR = $(PWD)
LD_LIBRARY_PATH=/opt/toolchains/gcc-linaro-6.2.1-2016.11-i686_aarch64-linux-gnu/lib/
endif
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CCDIR)aarch64-linux-gnu- -C $(KSRC) M=$(CUR_DIR) modules
clean:
	make CROSS_COMPILE=$(CCDIR)aarch64-linux-gnu- -C $(KSRC) M=$(CUR_DIR) clean
