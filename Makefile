
all : final.elf
	
final.elf : libinout.so main.o myProject.h
	gcc main.o -linout -L. -o final.elf

libinout.so : 1.o 2.o myProject.h
	gcc 1.o 2.o -o libinout.so -shared -fPIC
	
1.o : 1.c myProject.h
	gcc 1.c -c -o 1.o -shared -fPIC

2.o : 2.c myProject.h
	Gcc 2.c -c -o 2.o -shared -fPIC

main.o : main.c

rm : 
	rm *.o
	rm *.elf
	em *.so


