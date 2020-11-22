all: Temperaturetest

Temperaturetest : libMyPeri.a Temperaturetest.c Temperature.h
	arm-linux-gnueabi-gcc Temperaturetest.c -l MyPeri -L. -o Temperaturetest -lpthread
libMyPeri.a : button.o led.o buzzer.o fnd.o ColorLed.o Temperature.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o button.o buzzer.o fnd.o ColorLed.o Temperature.o
fnd.o : fnd.c fnd.h
	arm-linux-gnueabi-gcc -c fnd.c -o fnd.o
buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -c buzzer.c -o buzzer.o
button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -lpthread -o button.o
led.o: led.h led.c
	arm-linux-gnueabi-gcc -c led.c -o led.o
ColorLed.o: ColorLed.h ColorLed.c
	arm-linux-gnueabi-gcc -c ColorLed.c -o ColorLed.o
Temperature.o: Temperature.h Temperature.c
	arm-linux-gnueabi-gcc -c Temperature.c -o Temperature.o
rm : 
	rm *.o
	rm libMyPeri.a
