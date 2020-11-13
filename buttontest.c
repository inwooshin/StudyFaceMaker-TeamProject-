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


int main(int argc, char *argv[])
{
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
			case KEY_VOLUMEUP: printf("Volume up key):"); break;
			case KEY_HOME: printf("Home key):"); break;
			case KEY_SEARCH: printf("Search key):"); break;
			case KEY_BACK: printf("Back key):"); break;
			case KEY_MENU: printf("Menu key):"); break;
			case KEY_VOLUMEDOWN: printf("Volume down key):"); break;
			}
			if ( B.pressed ) printf("pressed\n");
			else printf("released\n");
		}
		else ;
		
	}
	
	
	buttonExit();
	return 0;
} 
