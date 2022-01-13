#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "textlcd.h"
#define TEXTLCD_DRIVER_NAME "/dev/peritextlcd"

int text(const char *str1 , const char *str2)
{
	unsigned int linenum = 0;
	stTextLCD stlcd; // stTextLCD 구조체를 가지고 드라이버와 인터페이스
	int fd;
	int len;
	
	memset(&stlcd,0,sizeof(stTextLCD)); // 구조체 초기화
	
	stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	len = strlen(str1);
	
	if ( len > COLUMN_NUM)
		memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, COLUMN_NUM);
	else
		memcpy(stlcd.TextData[stlcd.cmdData - 1], str1, len);
	
	stlcd.cmd = CMD_WRITE_STRING;
	fd = open(TEXTLCD_DRIVER_NAME,O_RDWR); // open driver
	
	if ( fd < 0 ) {
		perror("driver (//dev//peritextlcd) open error.\n");
		return 1;
	}
	
	write(fd,&stlcd,sizeof(stTextLCD)); //첫째줄
	
	stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	len = strlen(str2);
	
	if ( len > COLUMN_NUM)
		memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, COLUMN_NUM);
	else
		memcpy(stlcd.TextData[stlcd.cmdData - 1], str2, len);
	
	stlcd.cmd = CMD_WRITE_STRING;

	write(fd,&stlcd,sizeof(stTextLCD)); //두번째 줄
	
	close(fd);
	
	return 0;
}
