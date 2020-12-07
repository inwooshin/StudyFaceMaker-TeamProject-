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
//등등 헤더파일 설정해줘야함


hourAndMinute a[10],up;
int i = 0 , countOut = 0, goalOut = 0, all = 1000000, downOut = 0, first = 1, upAll;
int allStudy = 10;
int msgID; // 공부시간을 저장할 변수
char allStudyToChar[100];
BUTTON_MSG_T B;

int goalstudy(void);

int countup(void) 
{
	int on = 0;
	int returnValue;
	
	text("COUNT UP","");
	bitmainfunc("countup.bmp");
	
	
  while(1){
	  if(on){
		  if(first){
			first = 0;
			up.hour = 0;
			up.min = 0;
			up.sec = 0;
			  }
		  up.sec += 1;
		  if(up.sec > 59){ up.min += 1; up.sec = 0;}
		  if(up.min > 59){ up.hour += 1; up.min = 0;}
		  
		  upAll = 1000000 + up.hour * 10000 + up.min * 100 + up.sec;
		  fnd(upAll,MODE_STATIC_DIS);
		  
		  returnValue = 1;
		  while(returnValue > 0) 
			returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
		  sleep(1);
		  }
	
		returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
		
		if(B.type == EV_KEY){
			bitmainfunc("countup.bmp");
			if ( B.pressed ) {
				switch(B.keyInput)
			{
			case KEY_HOME:
				on ^= 1; if(on) text("COUNT UP", "play");	
				else text("COUNT UP" , "stop");  break;
			case KEY_BACK:if(!on){ first = 1; text("COUNT UP","reset");
				fnd(upAll,MODE_STATIC_DIS); }
				else text("COUNT UP","click after stop");
				break;
			case KEY_SEARCH:  break;
			case KEY_MENU:  break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN:
			bitmainfunc("timer.bmp");
			text("Timer Menu",""); return 0; break;
			}
			}
		}
		
	}
   
   return 0;
}

int countdown(void) 
{
	goalstudy();
	
	double setSec = a[i].hour * 3600 + a[i].min * 60 + a[i].sec;
	double own = setSec;
	
	text("COUNT DOWN", "");
	bitmainfunc("countdown2.bmp");
	
	int bit = 0x01;
	int light = 0;
	int on = 0;
	int once = 0;
	int returnValue;
	int percent;
	int j = 7;
	countOut = 0;
	
     while(1){
		 if(light){ pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				ledread("0x00");
				light = 0;
		}
		 if(on){
		 a[i].sec -= 1;
		 if(a[i].sec < 0) {a[i].sec = 59; a[i].min -= 1;}
		 if(a[i].min < 0) {a[i].min = 59; a[i].hour -= 1;}
		  
		all = 1000000 + a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
		fnd(all, MODE_STATIC_DIS);
		while(returnValue > 0) 
			returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
		
		double setSec = a[i].hour * 3600 + a[i].min * 60 + a[i].sec;
		percent = (setSec / own) / 0.125;
		
		if(percent == j){ 
			char toString[4];
			if(j == 7) ledread("0x01");
			else if(j == 6) ledread("0x03");
			else if(j == 5) ledread("0x07");
			else if(j == 4) ledread("0x0F");
			else if(j == 3) ledread("0x1F");
			else if(j == 2) ledread("0x3F");
			else if(j == 1) ledread("0x7F");
			else ledread("0xFF");
			
			j--;
			
		} 
		 
		sleep(1);
		
		}
		
		returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		
		if(B.type == EV_KEY){
			if ( B.pressed ) {
				light = 1;
				bitmainfunc("countdown2.bmp");
				switch(B.keyInput)
			{
			case KEY_HOME: 
				if(all > 1000000) on ^= 1; if(on) text("COUNT DOWN", "play");	
				else text("COUNT DOWN" , "stop");
			 break;
			case KEY_BACK: 	break;
			case KEY_SEARCH:  break;
			case KEY_MENU:  break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN: 
			bitmainfunc("timer.bmp");
			text("Timer Menu","");
			pwmSetPercent(0,0);
			pwmSetPercent(0,1);
			pwmSetPercent(0,2);
			countOut = 1;
			ledread("0x00");
			 return 0; break;
			}
				}
		}
		
		if(all == 1000000){
			on = 0;
			
			buzzerPlaySong(musicScale[0]);
			usleep(500000);
			buzzerPlaySong(musicScale[2]);
			usleep(500000);
			buzzerPlaySong(musicScale[4]);
			usleep(500000);
			buzzerPlaySong(musicScale[7]);
			usleep(500000);
			
			bitmainfunc("goal.bmp");
			
			text("COUNT DOWN", "         End..!!");
	

			for(int i = 0 ; i < 5 ; i++){	
				pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				usleep(500000);
				pwmSetPercent(0,0);
				pwmSetPercent(100,1);
				pwmSetPercent(100,2);
				sleep(1);
			}
			
			all = 0;
		}
	}
		
   return 0;
   
}

int timer(void) //시간메뉴설정 
{
	bitmainfunc("timer.bmp");
	//tft로 메뉴구현
    //1.카운트업 2.카운트다운 3.현재시간
    
	text("Timer Menu","");

	countOut = 0;

	while(1){
		if(countOut){countOut = 0; text("Timer Menu","");}
		
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			if ( B.pressed ){
			bitmainfunc("timer.bmp");
			switch(B.keyInput)
			{
			case KEY_HOME: countup();	break;
			case KEY_BACK: countdown(); break;
			case KEY_SEARCH:  
				fnd(0, MODE_TIME_DIS);
				break;
			case KEY_VOLUMEDOWN: 
			
			countOut = 1;
			bitmainfunc("MainMenu.bmp");
			text("main menu", "");
			 return 0; break;
			}
			}
		}
	}

	return 0;
}

int daystudy(void) //일일 학습량 확인
{
	
	
	text("1","daystudy");
	bitmainfunc("daily.bmp");
   //전역변수에 들어가있는 시간 출력
   //4는 예시고 stop 버튼 받으면
   //메인함수로 복귀
      return 0;
}

int weekstudy(void)
{
	text("2","weekstudy");
	bitmainfunc("weekly.bmp");
   //주간총 누적시간을 계산하고 새로운 전역변수에 저장해야댐
   //전역변수에 들어있는 시간 출력
   //4는 예시고 stop 버튼 받으면
   //메인함수로 복귀
      
      return 0; 
}

int goalstudy(void)
{   
	text("Goal","Please set time");
	bitmainfunc("countdown.bmp");
	all = 0 ;
	
	a[i].hour =0;
	a[i].min = 0;
	a[i].sec = 0;
	
  while(1){
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			
			if ( B.pressed ) {
				switch(B.keyInput)
			{
			case KEY_HOME: a[i].hour++; 
			if(a[i].hour > 59) 
				a[i].hour = 0;
			all = 1000000 + a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
			fnd(all,MODE_STATIC_DIS); break;
			case KEY_BACK: if(a[i].hour > 0)	a[i].hour--; 
				else break;
				all = 1000000 + a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
				fnd(all,MODE_STATIC_DIS);	break;
			case KEY_SEARCH: a[i].min++; 
				if(a[i].min > 59) 
					a[i].min = 0;
			all = 1000000 + a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
			fnd(all,MODE_STATIC_DIS); break;
			case KEY_MENU: if(a[i].min > 0)	a[i].min--; 
				all = 1000000 +a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
			fnd(all,MODE_STATIC_DIS); break;
			case KEY_VOLUMEUP: 
			a[i].sec++;
			if(a[i].sec > 59) a[i].sec = 0;
			all = 1000000 +a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
			fnd(all,MODE_STATIC_DIS);
			 break;
			case KEY_VOLUMEDOWN:
			 countOut = 1; return 0; break;
			}
				}
		}
		
	}

   
   
   return 0;
}

int setInit(void){
	
	bitmainfunc("start.bmp");
	
    pwmLedInit();
	ledLibInit();
	buzzerInit();
	buttonInit();
   
   text("Embedded System", "               ");
   sleep(2);
   text("60161818 SIW", "60162241 KHM");
   sleep(2);
	
	ledread("0x00");
	fnd(000000,MODE_STATIC_DIS );
	
	
	int ondo = 0;
   ondo = getTem();
   char temperature[20];
   sprintf(temperature, "temperature : %d", ondo);
   
   if(ondo < 20) text(temperature, "Cold to study");
   else if(ondo >= 20 && ondo < 26) text(temperature, "Enough to study!");
   else text(temperature, "Hot to study..!");
	
	return 0;
}


