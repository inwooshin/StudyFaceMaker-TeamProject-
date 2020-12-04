


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


#include "led.h"
#include "fnd.h"
#include "acc.h"
#include "button.h"
#include "buzzer.h"
#include "textlcd.h"
#include "ColorLed.h"
#include "Temperature.h"
//등등 헤더파일 설정해줘야함

typedef struct{
	int hour;
	int min ;
	int sec ;
} hourAndMinute;

hourAndMinute a[10];
int i = 0 , countOut = 0, goalOut = 0, all = 1000000, downOut = 0;
int allStudy = 10;
int msgID; // 공부시간을 저장할 변수
char allStudyToChar[100];
BUTTON_MSG_T B;

int countup() 
{
	
	
   while (1) {
      //textlcd에 버튼 어떤거 눌렀을때 어떻게 진행되는지 출력 해야댐
      //카운트업 시작, 카운트업 일시중지, 카운트 종료
      if (i == 1) //1은 예시고 카운트업 시작 버튼을 눌렀을때 카운트 업이 시작되야됨
      {
         switch (i) 
         {
         case 2: //2는 예시고 카운트일시정지 버튼을 눌렀을때
               //카운트가 일시정지되야됨
            break;
         case 1: //1은 예시고 카운트 시작 버튼을 눌렀을때
                //다시 카운트 시작
               if (i == 3) //3은 예시고 카운트 종료를 눌렀을때
                  ;//카운트를 종료하고 timer() 함수 호출
            break;
          case 3: //3은 예시고 카운트 종료를 눌렀을때
               ;//카운트를 종료하고 timer() 함수 호출
            break;
         }
      }
      else (i == 3) //3은 예시고 종료버튼 클릭시
         ;// timer 함수로 복귀
      
   }
   
   return 0;
}

int countdown() 
{
	text("COUNT DOWN", "");
	
	int light = 0;
	int on = 0;
	countOut = 0;
	
     while(1){
		 if(light){ pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				light = 0;
		}
		 if(on){
		 a[i].sec -= 1;
		 if(a[i].sec < 0) {a[i].sec = 59; a[i].min -= 1;}
		 if(a[i].min < 0) {a[i].min = 59; a[i].hour -= 1;}
		  
		all = 1000000 + a[i].hour * 10000 + a[i].min * 100 + a[i].sec;
		fnd(all, MODE_STATIC_DIS);
		sleep(1);
		
		}
		
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		
		if(B.type == EV_KEY){
			
			if ( B.pressed ) {
				light = 1;
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
			case KEY_VOLUMEDOWN: countOut = 1; return 0; break;
			}
				}
		}
		
		if(all == 1000000){
			on = 0;
			
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

int timer() //시간메뉴설정 
{
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
			switch(B.keyInput)
			{
			case KEY_HOME: countup();	break;
			case KEY_BACK: countdown(); break;
			case KEY_SEARCH:  
				fnd(0, MODE_TIME_DIS);
				break;
			case KEY_VOLUMEDOWN: countOut = 1; return 0; break;
			}
			}
		}
	}

	return 0;
}

int daystudy() //일일 학습량 확인
{
	
	
	text("1","daystudy");
   //전역변수에 들어가있는 시간 출력
   //4는 예시고 stop 버튼 받으면
   //메인함수로 복귀
      return 0;
}

int weekstudy()
{
	text("2","weekstudy");
   //주간총 누적시간을 계산하고 새로운 전역변수에 저장해야댐
   //전역변수에 들어있는 시간 출력
   //4는 예시고 stop 버튼 받으면
   //메인함수로 복귀
      
      return 0; 
}

int goalstudy()
{   
	text("Goal","Please set time");
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
			case KEY_VOLUMEDOWN: countOut = 1; return 0; break;
			}
				}
		}
		
	}

   
   
   return 0;
}

int setInit(void){
    pwmLedInit();
	ledLibInit();
	buzzerInit();
	buttonInit();
   
   text("Embedded System", "               ");
   //sleep(2);
   text("60161818 SIW", "60162241 KHM");
   //sleep(2);
	
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



int main(int argc, char* argv[]) {
   
   
	setInit();
	
	
	//tft로 메뉴출력
	text("main menu", "");
	buttonStart();
   
	int get = 1;
	while(get > 0){
		get = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
		};
   
	while(1){
		
		if(countOut){countOut = 0; text("main menu", "");}
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			if ( B.pressed ){
			switch(B.keyInput)
			{
			case KEY_VOLUMEDOWN: 
			text("program off..", "");
			 exit(0);
			break;
			case KEY_HOME: timer();	break;
			case KEY_BACK: daystudy(); break;
			case KEY_SEARCH:  weekstudy(); break;
			case KEY_MENU: goalstudy(); break;
			case KEY_VOLUMEUP:  break;
			}
		}
			//if ( B.pressed ) printf("pressed\n");
			//else printf("released\n");
		}
		
	}
   
   return 0;
}
