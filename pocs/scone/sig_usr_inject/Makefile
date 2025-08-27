obj-m += sig_usr_inject.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insmod: all
	sudo insmod sig_usr_inject.ko

log:
	tail -f /var/log/messages
