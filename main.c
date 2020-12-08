#include <getopt.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include <linux/fb.h>


#include "led.h"
#include "fnd.h"
#include "acc.h"
#include "button.h"
#include "buzzer.h"
#include "textlcd.h"
#include "ColorLed.h"
#include "Temperature.h"
#include "bitmap.h"
#include "embe.h"

hourAndMinute a[10],up;
int countOut;
int msgID; // 공부시간을 저장할 변수
char allStudyToChar[100];
BUTTON_MSG_T B;

int main(int argc, char* argv[]) {
   
	setInit(); //
	
	bitmainfunc("MainMenu.bmp");
	
	//tft로 메뉴출력
	text("main menu", "");
	buttonStart();
   
	int get = 1;
	while(get > 0){
		get = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
		};
   
	while(1){
		
		if(countOut){countOut = 0; text("Main Menu", "");}
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			if ( B.pressed ){
			bitmainfunc("MainMenu.bmp");
			switch(B.keyInput)
			{
			case KEY_HOME:
			 timer();	break;
			case KEY_BACK: daystudy(); break;
			case KEY_SEARCH:  weekstudy(); break;
			case KEY_MENU: break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN: 
			bitmainfunc("exit.bmp");
			text("program off..", "");
			 exit(0);
			break;
			}
		}
			//if ( B.pressed ) printf("pressed\n");
			//else printf("released\n");
		}
		
	}
   
   return 0;
}
