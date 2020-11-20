
all: textlcdtest

textlcdtest : libMyPeri.a textlcdtest.c textlcd.h
	arm-linux-gnueabi-gcc textlcdtest.c -l MyPeri -L. -o textlcdtest -lpthread
libMyPeri.a : button.o led.o buzzer.o fnd.o textlcd.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o fnd.o textlcd.o
textlcd.o : textlcd.c textlcd.h
	arm-linux-gnueabi-gcc -c textlcd.c -o textlcd.o
fnd.o : fnd.c fnd.h
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o

rm : 
	rm *.o
	rm libMyPeri.a
