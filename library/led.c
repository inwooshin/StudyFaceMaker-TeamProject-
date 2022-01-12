#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "led.h"

#define MAX_LED_NUM 8

static unsigned int ledValue = 0;
static int fd = 0;
unsigned int pdata=0;
int isWrite;

int ledOnOff (int ledNum, int onOff){

	int isWrite;
	
	int i=1;
	i = i<<ledNum;
	ledValue = ledValue& (~i);
	
	if (onOff !=0) ledValue |= i;
	
	isWrite = write (fd, &ledValue, 4);

	return isWrite;
}

int ledLibInit(void){

	fd=open("/dev/periled", O_WRONLY);
	
	ledValue = 0;

	return fd;
}

int ledLibExit(void){
	
	ledValue = 0;

	close(fd);

	return 0;
}

int ledread(const char data[]){
	
	pdata = strtol(data,NULL,16); //String을 16진수로 가정하고 integer형으로 변환
	printf("wrate data :0x%X\n", pdata);
	
	for(int i = 0; i < MAX_LED_NUM ; i++ ){
	
		if ( pdata & (0x01 << i)){
			isWrite = ledOnOff(i, 1);
			if(isWrite != 4)printf("write error");
		}
		else{
			isWrite = ledOnOff(i, 0);
			if(isWrite != 4)printf("write error");
		}
	}
}

int ledStatus(void){
	
	return ledValue;
}
