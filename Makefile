all : ledtest

ledtest : ledtest.c libMyperi.a
	arm-linux-gnueabi-gcc -o ledtest ledtest.c -l MyPeri -L. 

libMyperi.a : led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o
