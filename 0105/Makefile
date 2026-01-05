#=======================================
# Makefile for device module
#=======================================
KERVER := $(shell uname -r)
PWD := $(shell pwd)
CC := aarch64-linux-gnu-gcc 
KDIR := /usr/src/linux
obj-m := mydev_driver.o
TEST_TARGET = mydev_test
TEST_OBJS = mydev_test.o
TEST_SRCS = mydev_test.c

all: build $(TEST_TARGET)
build:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C$(KDIR) M=$(shell pwd) modules
#	make -C $(KDIR) SUBDIRS=$(PWD) modules
$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS)
clean:
	make -C$(KDIR) M=$(shell pwd) clean
#	rm -rf *.o *.ko *.mod.c .*.cmd *.order *.symvers .tmp*
	rm -f $(TEST_TARGET) $(TEST_OBJS)

