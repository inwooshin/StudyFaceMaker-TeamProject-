
all: buttontest

buttontest : buttontest.c libMyPeri.a button.h
	arm-linux-gnueabi-gcc buttontest.c -l MyPeri -L. -o buttontest -lpthread
libMyPeri.a : button.o led.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o

rm : 
	rm *.o
	rm libMyPeri.a
