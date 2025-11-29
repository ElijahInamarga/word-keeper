obj-m := word_keeper.o
word_keeper-objs := main_wk.o keeper_dev.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} modules
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=${PWD} clean
