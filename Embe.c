


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

int i = 0;
int allStudy = 10;
int msgID; // 공부시간을 저장할 변수
char allStudyToChar[100];
BUTTON_MSG_T B;

int countup() 
{
	/*
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
   */
   
   return 0;
}

int countdown() 
{
	/*
   while (1) 
   {
      //textlcd에 버튼 어떤거 눌렀을때 어떻게 진행되는지 출력 해야댐
      //시간설정, start
      switch (i) {

      case 1://1은 예시고 시간설정 버튼 클릭시
            //버튼클릭으로 시간을 설정하고 전역변수로 저장
         break;//종료 후 다시 메뉴로 나가기

      case 2: //2는 예시고 start 버튼 클릭시
            if (allStudy == 0) // 전역변수가 저장되있지 않을 시
               ;//저장되있는 시간이 없다고 오류띄우고 break

            //설정된 시간으로 카운트 다운 시작
            //이때 led로 시간/8 해서 가시적으로 볼수있게 설정해줘야댐
            //정상적으로 카운트다운 끝나면 전역변수에 저장하고 부저울리면서 종료 후 메뉴로 돌아가기
            if (i == 3)//3은 예시고 stop 버튼 클릭시
            {
               ;//카운트다운 일시정지
               if (i == 2) //2는 예시고  start 버튼 클릭시
                  ;//다시 카운트다운 시작
               if (i == 3) //3은 예시고  stop 버튼 클릭시
                  ;//세그먼트시간을 초기화하고 break 후 메뉴로 복귀
            }
         break;
         
      }
      if (i == 3) //3은 예시고 종료버튼 클릭시
         ;// timer 함수로 복귀
   }
   */
   
   return 0;
}

int timer() //시간메뉴설정 
{
   //textlcd에 메뉴출력 해야댐
    //1.카운트업 2.카운트다운 3.현재시간
    text("1. count up","2. count down 3.nod");

	/*
   while (1) {
      switch (i)
      {
      case 1:
         countup();
         break;
      case 2:
         countdown();
         break;
      case 3:
         //현재시간은 따른함수쓰지말고 걍 이 안에서 프린트 하면 되지 않을까..?
         break;
      }
      if (i==4) //4는 예시고 종료버튼 받으면 빠져나가게하기
         break;
   }
   */

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
	
	text("3","goalstudy");
	
	/*
   while (1) {
      //textlcd에 메뉴출력 해야댐
      //1.목표시간설정 2.목표시간달성여부
      switch (i) 
      {
      case 1:// 1은 예시고 목표시간설정 버튼 클릭시
            //버튼을 활용해 목표시간을 설정후 전역변수에 저장
         break;
      case 2: //2는 예시고 목표시간 달성여부 버튼 클릭시
            //목표시간 변수와 현재공부시간 변수를 동시에 출력
         break;
      }

      if (i == 4)//4는 예시고 stop 버튼 받으면
            ;//메인함수로 복귀
   }
   */
   
   return 0;
}

int setInit(void){
    pwmLedInit();
	ledLibInit();
	buzzerInit();
	buttonInit();
   
   text("Embedded System", "               ");
   sleep(2);
   text("60161818 SIW", "60162241 KHM");
   sleep(2);
	
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
	
	buttonStart();
   
	while(1){
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			switch(B.keyInput)
			{
			case KEY_HOME: timer();	break;
			case KEY_BACK: daystudy(); break;
			case KEY_SEARCH:  weekstudy(); break;
			case KEY_MENU: goalstudy(); break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN: text("program off..", ""); exit(0); break;
			}
			if ( B.pressed ) 
		}
		
	}
   
   return 0;
}
