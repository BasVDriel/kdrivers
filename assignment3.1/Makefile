ifneq ($(KERNELRELEASE),)
	obj-m := kmod.o

else
	KERNELDIR ?= \
	/lib/modules/`uname -r`/build/
	PWD := `pwd`

default: modules test

test: test.c
	$(CC) $(CFLAGS) -o $@ $<

modules:
	$(MAKE) -C $(KERNELDIR) \
	M=$(PWD) modules

endif

clean:
	rm -f *.ko *.o Module* *mod* test
