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
#include <time.h>

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

time_t tnow; //현재 시간의 초를 받아와주기 위한 변수
struct tm * t; //초 시간을 현재의 날짜로 변환해주기위한 구조체 변수
hourAndMinute a,up; //카운트 다운 구조체 변수와 업 구조체 변수
int  goalOut = 0, all = 1000000, first = 1, upAll = 1000000;
	//all 변수의 값으로 fnd에 출력시키기 위한 기본값, 첫번째로 업을 했을 때 구분하기 위한 변수
	//upAll 함수를 나갔다가 다시 들어와도 업한 시간이 저장되어있을 수 있도록 전역변수 선언
int firstGoal = 1;	//목표 시간을 설정 시 처음을 구분
int fp;	//파일을 여러 함수에서 쓸 수 있도록 전역 변수 선언
int allStudy = 0;	//총 공부량 저장
int msgID; // 공부시간을 저장할 변수
double downSec, own;	// 목표 달성 비율에 따른 led 출력을 위한 변수
BUTTON_MSG_T B;	//버튼 입력 구조체

int goalstudy(void);

int countup(void) 
{
	int on = 0;	//start 버튼을 눌렀을 경우를 구분하기 위한 변수
	int returnValue;	
	int point; //read 한 만큼의 offset 값을 읽어오는 변수
	int flag; //파일 중에 일치하는 날짜를 발견했을 때 flag를 띄우기 위함
	char todayStudy[20], now[20], compareString[20];
		//오늘 공부한 시간을 초로 저장할 버퍼와, 오늘 시간, 비교할 문자열을 저장할 버퍼
		
	time(&tnow); //시간을 초단위로 받아옴
	t = (struct tm*)localtime(&tnow);	//현재 보편적인 시간으로 변경 (년 월 일)
	up.year = t->tm_year+1900; 
	up.month = t->tm_mon+1;
	up.day = t->tm_mday; //년 월 일을 구조체에서 받아옴
	
	text("COUNT UP",""); 
	bitmainfunc("countup.bmp");
	
	fnd(upAll,MODE_STATIC_DIS); //현재 얼마나 했는지 시간을 띄우거나 초기값 띄움
	
	while(1){
	  if(on){	//실행 버튼을 누르면
		  if(first){	//처음 시작할 경우에 0으로 시간 분 초 초기화
			first = 0;
			up.hour = 0;
			up.min = 0;
			up.sec = 0;
			  }
		  up.sec += 1;	//초 증가
		  if(up.sec > 59){ up.min += 1; up.sec = 0;}
		  if(up.min > 59){ up.hour += 1; up.min = 0;}
							//초 60 증가 시 1분 분 60 증가시 1시간
		  upAll = 1000000 + up.hour * 10000 + up.min * 100 + up.sec;
		  fnd(upAll,MODE_STATIC_DIS);	//더하고 띄움
		  
		  returnValue = 1;		
		  while(returnValue > 0) 
			returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
		  sleep(1);	
		  }	//쌓여있는 메세지 받음
	
		returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
						//버튼 입력을 스레드로 돌고있는 파일에서 받아옴
		
		if(B.type == EV_KEY){	
			if ( B.pressed ) {	
				switch(B.keyInput)
			{
			case KEY_HOME:	//홈 버튼 누를 시 on 을 xor 시켜서 플레이 중이면 스탑 스탑 중이면 플레이
				on ^= 1; if(on) text("COUNT UP", "play");	//플레이 하면 textlcd에 play 
				else text("COUNT UP" , "stop");  break;		//스탑하면 textlcd에 stop 출력
			case KEY_BACK:if(!on){ first = 1; text("COUNT UP","reset");	
				fnd(000000,MODE_STATIC_DIS); }	//스탑 한 채로 BACK 버튼이 눌리면 리셋
				else text("COUNT UP","click after stop");
				break;
			case KEY_SEARCH:  break;
			case KEY_MENU:  break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN:
			allStudy = up.hour * 3600 + up.min * 60 + up.sec;	//초단위로 공부한 총 시간 변경
			sprintf(now, "%dY %dM %dD", up.year, up.month, up.day);	//현재 공부한 시간을 문자열로
			lseek(fp, 0, SEEK_SET);	
			while(0 < (read(fp,compareString,13))){	//파일에서 날짜를 파일 끝까지 순차적으로 읽음
				if(strncasecmp(compareString, now, 12) == 0) {flag = 1; break;}
					//파일에서 읽어온 날짜를 공부한 날짜와 같은 것이 있는지 확인
				lseek(fp,7,SEEK_CUR);	//공부한 시간은 offset 건너뒤기
			}
			if(!flag){ //없으면 날짜와 공부시간 둘다 써주기
				write(fp, now, strlen(now));
				sprintf(todayStudy, "%06d\n", allStudy);
				write(fp, todayStudy, strlen(todayStudy));}
			else{	//있으면 시간만 써주기 초단위로 되어있는 시간을 시간 분으로 변경하고 써주기
				point = read(fp, compareString, 7);	//공부한 이전의 시간을 읽기
				int before = atoi(compareString); 	//이전 시간 문자열로
				before += allStudy;	
				sprintf(todayStudy, "%06d\n", before); //정수를 문자열로 바꿔줌 일반적인 형식을 위해 
															//6자리 정수형으로 통일
				lseek(fp, -point, SEEK_CUR);	//시간을 읽어준만큼 이전의 포인터로 돌아감
				write(fp, todayStudy, strlen(todayStudy));	//시간을 써줌
				}
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
	goalstudy();	//목표시간 설정
	
	fnd(all, MODE_STATIC_DIS);	//현재 목표시간 설정
	
	char now[20], todayStudy[20];
	downSec = a.hour * 3600 + a.min * 60 + a.sec;	
	own = downSec;	//목표시간을 초단위로 총량을 저장 후의 led 비율을 위해
	
	text("COUNT DOWN", "");
	bitmainfunc("countdown2.bmp");
	
	int point;
	int light = 0;
	int on = 0;
	int returnValue = 1;
	int percent, flag = 0;
	char compareString[20];
	
     while(1){
		 if(light){ //목표 달성 시 led를 사용했을 경우 목표 설정하고 재사용시 led를 꺼줌
				pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				ledread("0x00");
				light = 0;
		}
		 if(on){	//play 버튼을 누르면 실행
		 a.sec -= 1;
		 if(a.sec < 0) {a.sec = 59; a.min -= 1;}
		 if(a.min < 0) {a.min = 59; a.hour -= 1;}
		  
		all = 1000000 + a.hour * 10000 + a.min * 100 + a.sec;
		fnd(all, MODE_STATIC_DIS);
		while(returnValue > 0) 
			returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0,IPC_NOWAIT);
				//쌓여있는 메세지 없앰
		downSec = a.hour * 3600 + a.min * 60 + a.sec;	//현재 공부한 시간을 초로 변경
		percent = (8 * downSec) / own;	//처음 목표 설정량과 현재 공부량의 비율을 산정
		
			if(percent == 1) ledread("0x01");
			else if(percent == 2) ledread("0x03");
			else if(percent == 3) ledread("0x07");
			else if(percent == 4) ledread("0x0F");
			else if(percent == 5) ledread("0x1F");
			else if(percent == 6) ledread("0x3F");
			else if(percent == 7) ledread("0x7F");
			else if(percent == 8) ledread("0xFF");
			//	1/8 만큼 채워지면 led 하나씩 출력시킴
		 
		sleep(1);
		
		}
		
		returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		
		if(B.type == EV_KEY){
			if ( B.pressed ) {
				bitmainfunc("countdown2.bmp");
				switch(B.keyInput)
			{
			case KEY_HOME: //실행
				if(all > 1000000) on ^= 1; //실행중이면 스탑 스탑중이면 실행
				if(on) text("COUNT DOWN", "play");	//textlcd에 출력
				else text("COUNT DOWN" , "stop");
			 break;
			case KEY_BACK:	//리셋버튼
				text("COUNT DOWN" , "stop");	
				firstGoal = 1;		//처음 설정할 경우의 옵션을 설정해 준다
				goalstudy();	//리셋해준다.
			 	break;
			case KEY_SEARCH:  break;
			case KEY_MENU:  break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN: //종료
			bitmainfunc("timer.bmp");
			text("Timer Menu","");
			pwmSetPercent(0,0);
			pwmSetPercent(0,1);
			pwmSetPercent(0,2);
			ledread("0x00");	//만약 사용중인 led가 있으면 전부 꺼줌
			allStudy = (own - (a.hour * 3600 + a.min * 60 + a.sec));	//목표 설정 시간에서 실행 시간을
																		//초단위로 빼고 저장
			sprintf(now, "%dY %dM %dD\n", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
					//현재 날짜를 저장
			
			lseek(fp, 0, SEEK_SET);
			printf("\n\n");
			
			while(0 < (read(fp,compareString,13))){	//파일에서 날짜 읽고
				printf("%sstrlen : %d\n", compareString, strlen(compareString));
				if(strncasecmp(compareString, now, 13) == 0) {flag = 1; break;}
				
				lseek(fp,7,SEEK_CUR);
			}
			printf("\n flag : %d\n", flag);
			if(!flag){ //해당하는 날짜 없을 경우 날짜 및 공부한 시간 써주기
				write(fp, now, strlen(now));
				sprintf(todayStudy, "%06d\n", allStudy);
				write(fp, todayStudy, strlen(todayStudy));}
			else{//해당하는 날짜 있을 경우 공부한 시간만 써주기
				point = read(fp, compareString, 7);
				int before = atoi(compareString);
				printf("\n\nbefore : %d\n\n", before);
				before += allStudy;
				sprintf(todayStudy, "%06d\n", before);
				lseek(fp, -point, SEEK_CUR);
				write(fp, todayStudy, strlen(todayStudy));}
			return 0; break;
			}
			}
		}
		
		if(all == 1000000){ //목표 시간이 완료되었을 경우
			on = 0;	//실행하던 것을 멈춘다.
			firstGoal = 1;	//목표 설정을 위해서 다시 처음 목표를 설정하는 상태로 바꾼다.
			
			bitmainfunc("goal.bmp");
			
			text("COUNT DOWN", "         End..!!");
	

			buzzerPlaySong(musicScale[0]);
				pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				usleep(500000);
			buzzerPlaySong(musicScale[2]);
				pwmSetPercent(0,0);
				pwmSetPercent(100,1);
				pwmSetPercent(100,2);
				usleep(500000);
			buzzerPlaySong(musicScale[4]);
				pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				usleep(500000);
			buzzerPlaySong(musicScale[7]);
				pwmSetPercent(0,0);
				pwmSetPercent(100,1);
				pwmSetPercent(100,2);
				usleep(500000);
			buzzerStopSong();
				pwmSetPercent(0,0);
				pwmSetPercent(0,1);
				pwmSetPercent(0,2);
				usleep(500000);
				pwmSetPercent(0,0);
				pwmSetPercent(100,1);
				pwmSetPercent(100,2);
			//도레미솔 0.5초씩 울리면서 colorled를 cyan으로 깜빡인다.
			
			
			all = 0;
		}
	}
		
   return 0;
   
}

int currentTime(void){	//현재 시간 출력
	
	bitmainfunc("current.bmp");
	text("Current Time", "");
	fnd(0, MODE_TIME_DIS);
	
	
	while(1){
		
		fnd(0, MODE_TIME_DIS);
		usleep(200000);//0.2초마다 현재 시간 fnd에 출력
		
	int returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		if(B.type == EV_KEY){
			if ( B.pressed ){
				switch(B.keyInput)
				{
				case KEY_VOLUMEDOWN: //종료
				fnd(000000,MODE_STATIC_DIS );
				text("Timer Menu","");
				bitmainfunc("timer.bmp");
				return 0; break;
				}
			}
		}
	}
	
}

int timer(void) //타이머 기능 선택 메뉴 
{
	bitmainfunc("timer.bmp");
	//tft로 메뉴구현
    //1.카운트업 2.카운트다운 3.현재시간
    
	text("Timer Menu","");


	while(1){
		
		int returnValue = 0;	
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
			// 버튼 메세지 읽어오기
		
		if(B.type == EV_KEY){
			if ( B.pressed ){
			switch(B.keyInput)
			{
			case KEY_HOME: //카운트 업
				countup();	break;
			case KEY_BACK: //카운트 다운
			 countdown(); break;
			case KEY_SEARCH:  //현재 시간
				currentTime();
				break;
			case KEY_VOLUMEDOWN: //종료
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
	bitmainfunc("daily.bmp");
	
	char now[16], todayStudy[20];
	char compare[20], doWork[20], HWString[20];
	
	time(&tnow);
	t = (struct tm*)localtime(&tnow);
	int ty = t->tm_year+1900;
	int tm =t->tm_mon+1;
	int td =t->tm_mday;
	int flag, number, hour, min, point, readValue;
	sprintf(now, "%dY %dM %dD", ty, tm, td);
	//현재 시간 저장
	
	lseek(fp, 0, SEEK_SET); //처음부터
			
			while(0 < (readValue = read(fp,compare,13))){ //날짜 읽고
				if((strncasecmp(compare, now, 12)) == 0) { //비교하기 
					flag = 1; break;
				}
				
				lseek(fp,7,SEEK_CUR);
			}
			printf("\n flag : %d\n", flag);
			if(!flag){ //오늘의 날짜가 없을 경우 오늘 공부 안함 출력
				text(now, "today did not study..!");}
			else{ //오늘의 날짜가 있을 경우 오늘 공부한 시간을 파일에서 읽어오고 초단위를 시간 분으로 바꿔서
				read(fp, HWString, 7);	//출력
				printf("\n\nstring : %s\n\n", HWString);
				number = atoi(HWString);
				hour = number / 3600;
				min = (number % 3600) / 60;
				sprintf(doWork, "        %02dH %02dM", hour, min);
				text(now, doWork);	//해당 정보를 textlcd에 출력...!!
				}
	
	while(1){
		
	int returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		if(B.type == EV_KEY){
			if ( B.pressed ){
				switch(B.keyInput)
				{
				case KEY_VOLUMEDOWN: //종료
				text("Main Menu","");
				bitmainfunc("MainMenu.bmp");
				return 0; break;
				}
			}
		}
	}
	
      return 0;
}

int weekstudy(void)
{
	bitmainfunc("weekly.bmp");
	
	char compare[20], HWString[20], allString[20], allnow[20];
	char now[7][20], doWork[7][20];
	
	int j = 0, next = 0, all = 0, on_all = 1, firstDay= 1;
	time(&tnow);
	t = (struct tm*)localtime(&tnow);
	int ty = t->tm_year+1900;
	int tm =t->tm_mon+1;
	int td =t->tm_mday;
	int flag = 1, number, hour, min, point, readValue;
	sprintf(now[0], "%dY %dM %dD", ty, tm, td);
	//현재 시간 저장
	
	while(flag && j < 7){	//파일에 현재시간부터 일주일간의 날짜가 있는 비교
		flag = 0;
		lseek(fp, 0, SEEK_SET);
		while(0 < (readValue = read(fp,compare,13))){
			printf("%sstrlen : %d\n", compare, strlen(compare));
			if((strncasecmp(compare, now[j], 12)) == 0) {
				flag = 1; 
				read(fp, HWString, 7);
				number = atoi(HWString);
				all += number;
				hour = number / 3600;
				min = (number % 3600) / 60;
				sprintf(doWork[j], "        %02dH %02dM", hour, min);
				printf("\n doWork : %s\n", doWork[j]);
				
				j++;
				if(j < 7){
				tnow -= 86400;
				t = (struct tm*)localtime(&tnow);
				ty = t->tm_year+1900;
				tm =t->tm_mon+1;
				td =t->tm_mday;
				sprintf(now[j], "%dY %dM %dD", ty, tm, td);
				printf("\n\n%s\n\n", now[j]);
				break;
			}
			}
			lseek(fp,7,SEEK_CUR);
		}
	}
	
	time(&tnow);
	t = (struct tm*)localtime(&tnow);
	int allm =t->tm_mon+1;
	int alld =t->tm_mday;
	//1주일을 합친 총시간의 시작 날짜와 
	
	hour = all / 3600;
	min = (all % 3600) / 60;
	sprintf(allnow, "%dM %dD - %dM %dD", tm, td, allm, alld);
	sprintf(allString, "        %02dH %02dM", hour, min);
	
	text(allnow, allString);
	//현재 날짜와 1주일 전 날짜와 그리고 1주일 동안 공부한 총량을 textlcd에 출력
		
	while(1){
		
		
	int returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		if(B.type == EV_KEY){
			if ( B.pressed ){
				switch(B.keyInput)
				{
			case KEY_HOME: //1주일 총량 띄움
				if(!on_all){	//1주일 총량을 띄우지 않은 상태에서 home 버튼을 누르면 다시 띄움
				text(allnow, allString);
					on_all = 1;
					firstDay = 1;
					next = 0;
				}
				else break;
				break;
			case KEY_BACK:	//최근 사용량부터 textlcd에 띄움 그리고 왼쪽키의 역할을 수행
				on_all = 0;
				if(firstDay){
					firstDay = 0;
					next = 0;
					text(now[next], doWork[next]);
					break;
					}
				next--;
				if(next >= 0 && next < j) {text(now[next], doWork[next]); break;}
				else next = 0;
				break;
			case KEY_SEARCH: //1주일 전 사용량부터 textlcd에 띄움 오른쪽 키의 역할을 수행
				on_all = 0;
				if(firstDay){
					firstDay = 0;
					next = j-1;
					text(now[next], doWork[next]);
					break;
					}
				next++;
				if(next >= 0 && next < j) {	text(now[next], doWork[next]); break;}
				else next = j-1;
				break;
			case KEY_MENU: break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN: //종료
				text("Main Menu","");
				bitmainfunc("MainMenu.bmp");
			return 0; break;
				}
			}
		}
	}
	
      return 0;
}

int goalstudy(void)
{   
	if(!firstGoal){return 0;}	//이미 설정한 목표가 있을 경우 리턴
	
	text("Goal","Please set time");
	bitmainfunc("countdown.bmp");
	all = 0 ;
	
	if(firstGoal == 1){	//처음 목표 설정 시 초기화
		a.hour =0;
		a.min = 0;
		a.sec = 0;
		
		firstGoal = 0;
	}
	
	time(&tnow);
	t = (struct tm*)localtime(&tnow);
	a.year = t->tm_year+1900;
	a.month = t->tm_mon+1;
	a.day = t->tm_mday;
	//시작 시 현재 시간 받아옴
	
	fnd(000000, MODE_STATIC_DIS);
	
  while(1){
	
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			
			if ( B.pressed ) {
				switch(B.keyInput)
			{
			case KEY_HOME: a.hour++; //시 추가
			if(a.hour > 59) 
				a.hour = 0;
				all = 1000000 + a.hour * 10000 + a.min * 100 + a.sec;
				fnd(all,MODE_STATIC_DIS); break;
			case KEY_BACK: if(a.hour > 0)	a.hour--; //시 감소
				else break;
				all = 1000000 + a.hour * 10000 + a.min * 100 + a.sec;
				fnd(all,MODE_STATIC_DIS);	break;
			case KEY_SEARCH: a.min++; //분 추가
				if(a.min > 59) 
					a.min = 0;
				all = 1000000 + a.hour * 10000 + a.min * 100 + a.sec;
				fnd(all,MODE_STATIC_DIS); break;
			case KEY_MENU: if(a.min > 0)	a.min--; //분 감소
				all = 1000000 +a.hour * 10000 + a.min * 100 + a.sec;
				fnd(all,MODE_STATIC_DIS); break;
			case KEY_VOLUMEUP: // 초 증가
				a.sec++;
				if(a.sec > 59) a.sec = 0;
				all = 1000000 +a.hour * 10000 + a.min * 100 + a.sec;
				fnd(all,MODE_STATIC_DIS);
			 break;
			case KEY_VOLUMEDOWN: //설정 완료
				text("COUNT DOWN", "");
				bitmainfunc("countdown2.bmp");
			  return 0; break;
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
   //각 기기 초기 설정
   
   fp = open("log.txt", O_RDWR | O_CREAT, 0666);
   //로그 파일(이전 공부량 저장 파일) 읽어오기
   
   text("Embedded System", "               ");
   sleep(1);
   text("60161818 SIW", "60162241 KHM");
   sleep(2);
	
	ledread("0x00");
	fnd(000000,MODE_STATIC_DIS );
	
	int ondo = 0;
   ondo = getTem() - 5;
   char temperature[20];
   sprintf(temperature, "temperature : %d", ondo);
   //온도를 읽어오고 textlcd 에 띄우기 위해서 문자열 저장
   
   if(ondo < 20) 
    { bitmainfunc("cold.bmp"); text(temperature, "Cold to study");}
   else if(ondo >= 20 && ondo < 26)
    {bitmainfunc("nice.bmp"); text(temperature, "Nice to study!");}
   else 
    {bitmainfunc("hot.bmp"); text(temperature, "Hot to study..!");}
    //온도에 맞는 안내메세지를 textlcd에 띄움
    
	sleep(2);
	
	return 0;
}

int setExit(void){
	pwmInactiveAll();
	ledLibExit();
	buzzerExit();
	buttonExit();
	//열어놓은 파일 전부 종료
	
	close(fp);//log 파일도 종료
	
	
}

