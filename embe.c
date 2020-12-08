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

time_t tnow;
struct tm * t;
hourAndMinute a[10],up;
int i = 0 , countOut = 0, goalOut = 0, all = 1000000, downOut = 0, first = 1, upAll;
int firstGoal = 1;
int fp;
int allStudy = 0;
int msgID; // 공부시간을 저장할 변수
char allStudyToChar[100];
double setSec, own;
BUTTON_MSG_T B;

int goalstudy(void);

int countup(void) 
{
	int on = 0;
	int returnValue;
	int point;
	int flag;
	char todayStudy[20], now[20], compareString[20];
	
	time(&tnow);
	t = (struct tm*)localtime(&tnow);
	up.year = t->tm_year+1900;
	up.month = t->tm_mon+1;
	up.day = t->tm_mday;
	
	text("COUNT UP","");
	bitmainfunc("countup.bmp");
	
	fnd(upAll,MODE_STATIC_DIS);
	
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
				fnd(000000,MODE_STATIC_DIS); }
				else text("COUNT UP","click after stop");
				break;
			case KEY_SEARCH:  break;
			case KEY_MENU:  break;
			case KEY_VOLUMEUP:  break;
			case KEY_VOLUMEDOWN:
			allStudy = up.hour * 3600 + up.min * 60 + up.sec;
			printf("\nallStudy : %d\n", allStudy);
			sprintf(now, "%dY %dM %dD", up.year, up.month, up.day);
			lseek(fp, 0, SEEK_SET);
			while(0 < (read(fp,compareString,13))){
				printf("%sstrlen : %d\n", compareString, strlen(compareString));
				if(strncasecmp(compareString, now, 12) == 0) {flag = 1; break;}
				lseek(fp,7,SEEK_CUR);
			}
			if(!flag){ 
				write(fp, now, strlen(now));
				sprintf(todayStudy, "%06d\n", allStudy);
				write(fp, todayStudy, strlen(todayStudy));}
			else{
				point = read(fp, compareString, 7);
				int before = atoi(compareString);
				printf("\n\nbefore : %d\n\n", before);
				allStudy += before;
				sprintf(todayStudy, "%06d\n", allStudy);
				lseek(fp, -point, SEEK_CUR);
				write(fp, todayStudy, strlen(todayStudy));
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
	goalstudy();
	
	fnd(all, MODE_STATIC_DIS);
	
	char now[20], todayStudy[20];
	setSec = a[i].hour * 3600 + a[i].min * 60 + a[i].sec;
	own = setSec;
	
	text("COUNT DOWN", "");
	bitmainfunc("countdown2.bmp");
	
	int bit = 0x01;
	int point;
	int light = 0;
	int on = 0;
	int once = 0;
	int returnValue;
	int percent, flag = 0;
	int j = 7;
	
	char compareString[20];
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
		
		setSec = a[i].hour * 3600 + a[i].min * 60 + a[i].sec;
		percent = (setSec / own) / 0.125;
		
			if(percent == 6) ledread("0x01");
			else if(percent == 5) ledread("0x03");
			else if(percent == 4) ledread("0x07");
			else if(percent == 3) ledread("0x0F");
			else if(percent == 2) ledread("0x1F");
			else if(percent == 1) ledread("0x3F");
			else if(percent == 0) ledread("0x7F");
			if(all == 1000000) ledread("0xFF");
			
		 
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
				if(all > 1000000) on ^= 1; 
				if(on) text("COUNT DOWN", "play");	
				else text("COUNT DOWN" , "stop");
			 break;
			case KEY_BACK:
				text("COUNT DOWN" , "stop");
				firstGoal = 1;
				goalstudy();
			 	break;
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
			allStudy = (own - (a[i].hour * 3600 + a[i].min * 60 + a[i].sec));
			sprintf(now, "%dY %dM %dD\n", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
			printf("allStudy : %d", allStudy);
			
			lseek(fp, 0, SEEK_SET);
			printf("\n\n");
			
			while(0 < (read(fp,compareString,13))){
				printf("%sstrlen : %d\n", compareString, strlen(compareString));
				if(strncasecmp(compareString, now, 13) == 0) {flag = 1; break;}
				
				lseek(fp,7,SEEK_CUR);
			}
			printf("\n flag : %d\n", flag);
			if(!flag){ 
				write(fp, now, strlen(now));
				sprintf(todayStudy, "%06d\n", allStudy);
				write(fp, todayStudy, strlen(todayStudy));}
			else{
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
		
		if(all == 1000000){
			on = 0;
			firstGoal = 1;
			
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
			
			all = 0;
		}
	}
		
   return 0;
   
}

int currentTime(void){
	
	bitmainfunc("current.bmp");
	text("Current Time", "");
	fnd(0, MODE_TIME_DIS);
	
	
	while(1){
		
		fnd(0, MODE_TIME_DIS);
		
	int returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		if(B.type == EV_KEY){
			if ( B.pressed ){
				switch(B.keyInput)
				{
				case KEY_VOLUMEDOWN: 
				fnd(000000,MODE_STATIC_DIS );
				text("Timer Menu","");
				bitmainfunc("timer.bmp");
				return 0; break;
				}
			}
		}
	}
	
}

int timer(void) //시간메뉴설정 
{
	bitmainfunc("timer.bmp");
	//tft로 메뉴구현
    //1.카운트업 2.카운트다운 3.현재시간
    
	text("Timer Menu","");

	countOut = 0;

	while(1){
		
		int returnValue = 0;
		returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, 0);
		
		if(B.type == EV_KEY){
			if ( B.pressed ){
			switch(B.keyInput)
			{
			case KEY_HOME: countup();	break;
			case KEY_BACK: countdown(); break;
			case KEY_SEARCH:  
				currentTime();
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
	
	lseek(fp, 0, SEEK_SET);
	printf("\n\n");
			
			while(0 < (readValue = read(fp,compare,13))){
				printf("%sstrlen : %d\n", compare, strlen(compare));
				if((strncasecmp(compare, now, 12)) == 0) {
					flag = 1; break;
				}
				
				lseek(fp,7,SEEK_CUR);
			}
			printf("\n flag : %d\n", flag);
			if(!flag){ 
				text(now, "today did not study..!");}
			else{
				read(fp, HWString, 7);
				printf("\n\nstring : %s\n\n", HWString);
				number = atoi(HWString);
				hour = number / 3600;
				min = (number % 3600) / 60;
				printf("\n\n\nnumber : %d\n\n",number);
				sprintf(doWork, "        %02dH %02dM", hour, min);
				text(now, doWork);}
	
	while(1){
		
		
	int returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		if(B.type == EV_KEY){
			if ( B.pressed ){
				switch(B.keyInput)
				{
				case KEY_VOLUMEDOWN: 
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
	
	while(flag && j < 7){
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
	hour = all / 3600;
	min = (all % 3600) / 60;
	sprintf(allnow, "%dM %dD - %dM %dD", allm, alld, tm, td);
	sprintf(allString, "        %02dH %02dM", hour, min);
	
	text(allnow, allString);
	while(1){
		
		
	int returnValue = msgrcv(msgID, &B, sizeof(unsigned short) * 2 + sizeof(int), 0, IPC_NOWAIT);
		if(B.type == EV_KEY){
			if ( B.pressed ){
				switch(B.keyInput)
				{
			case KEY_HOME: 
				if(!on_all){
				text(allnow, allString);
					on_all = 1;
					firstDay = 1;
					next = 0;
				}
				else break;
				break;
			case KEY_BACK:
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
			case KEY_SEARCH: 
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
			case KEY_VOLUMEDOWN: 
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
	if(!firstGoal){return 0;}
	
	text("Goal","Please set time");
	bitmainfunc("countdown.bmp");
	all = 0 ;
	
	if(firstGoal == 1){
		a[i].hour =0;
		a[i].min = 0;
		a[i].sec = 0;
		
		firstGoal = 0;
	}
	
	time(&tnow);
	t = (struct tm*)localtime(&tnow);
	a[i].year = t->tm_year+1900;
	a[i].month = t->tm_mon+1;
	a[i].day = t->tm_mday;
	
	fnd(000000, MODE_STATIC_DIS);
	
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
   
   fp = open("log.txt", O_RDWR | O_CREAT, 0666);
   
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
   
   
   if(ondo < 20) 
    { bitmainfunc("cold.bmp"); text(temperature, "Cold to study");}
   else if(ondo >= 20 && ondo < 26)
    {bitmainfunc("nice.bmp"); text(temperature, "Nice to study!");}
   else 
    {bitmainfunc("hot.bmp"); text(temperature, "Hot to study..!");}
    
	
	
	sleep(2);
	
	return 0;
}


