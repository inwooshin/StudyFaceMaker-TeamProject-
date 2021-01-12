#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include "button.h"
#include "led.h"

#define LED_DRIVER_NAME "/dev/periled"
#define MAX_LED_NUM 8


int main(int argc, char *argv[])
{
	ledLibInit();
	
	BUTTON_MSG_T B;
	
	int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
	
	if (msgID == -1){
	printf ("Cannot get msgQueueID, Return!\r\n");
	return -1;
	}
	
	buttonInit();
	
	while(1){
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			switch(B.keyInput)
			{
			case KEY_HOME: ledOnOff(0x00,1);
			ledOnOff(0x01,1); 					
			ledOnOff(0x02,1); 					
			ledOnOff(0x04,1); 					
			ledOnOff(0x08,1); 					
			ledOnOff(0x0F,1); 					
			break;
			case KEY_BACK: ledOnOff(0x00,0); break;
			case KEY_SEARCH: ledOnOff(0x01,1); break;
			case KEY_MENU: ledOnOff(0x01,0); break;
			case KEY_VOLUMEUP: ledOnOff(0x02,1); break;
			case KEY_VOLUMEDOWN: ledOnOff(0x02,0);break;
			}
			if ( B.pressed ) printf("pressed\n");
			else printf("released\n");
		}
		else ;
		
	}
	
	
	buttonExit();
	return 0;
} 
