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

int countOut;
int msgID; // 공부시간을 저장할 변수
char allStudyToChar[100];
BUTTON_MSG_T B;

int main(int argc, char* argv[]) {
   
	setInit(); //각종 기기들을 키고 log.txt 파일을 오픈하며 초기 설정을 해준다.
	
	bitmainfunc("MainMenu.bmp"); //메인 메뉴 사진을 띄운다.
	
	text("Main Menu", "");
	buttonStart();	
   
	while(1){
		
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
			//버튼의 입력을 받아온다.
		
		if(B.type == EV_KEY){
			if ( B.pressed ){ //버튼이 눌렸을 경우
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
			setExit(); //종료 시 열어놓은 파일들을 전부 close 하고 종료한다.
			exit(0);
			break;
			}
		}
		}
		
	}
   
   return 0;
}
