obj-m += list.o

CFLAGS_list.o := -DDEBUG

ifndef KSRC
CUR_DIR = $(shell pwd)
else
CCDIR = /opt/toolchains/gcc-linaro-6.2.1-2016.11-i686_aarch64-linux-gnu/bin/
CUR_DIR = $(shell pwd)
LD_LIBRARY_PATH=/opt/toolchains/gcc-linaro-6.2.1-2016.11-i686_aarch64-linux-gnu/lib/
endif

odroid-c2:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CCDIR)aarch64-linux-gnu- -C $(KSRC) M=$(CUR_DIR) modules

x86:
	$(eval KSRC = /lib/modules/$(shell uname -r)/build)
	make -C $(KSRC) M=$(CUR_DIR) modules
clean:
	make -C $(KSRC) M=$(CUR_DIR) clean
