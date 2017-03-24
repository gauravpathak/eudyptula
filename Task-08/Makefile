obj-m += debug_fs.o

CFLAGS_debug_fs.o := -DDEBUG

ifndef KSRC
KSRC = /lib/modules/$(shell uname -r)/build
endif
all:
	make -C $(KSRC) M=$(PWD) modules
clean:
	make -C $(KSRC) M=$(PWD) clean
