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
// first read input device
#define INPUT_DEVICE_LIST "/dev/input/event" //실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음., ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices" //PPT에 제시된 "이 파일을 까보면 event? 의 숫자를 알수 있다"는 바로 그 파일
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"

BUTTON_MSG_T A;
int msgID, fd;
char buttonPath[200] = {0,};
pthread_t buttonTh_id;

int probeButtonPath(char *newPath){
	int returnValue = 0; //button에 해당하는 event#을 찾았나?
	int number = 0; //찾았다면 여기에 집어넣자
	FILE *fp = fopen(PROBE_FILE,"rt");
	while(!feof(fp)) {
	char tmpStr[200]; //200자를 읽을 수 있게 버퍼
	fgets(tmpStr,200,fp); //최대 200자를 읽어봄
	//printf ("%s",tmpStr);
	if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0){
		//printf("YES! I found!: %s\r\n", tmpStr);
		returnValue = 1; //찾음
	}
	
	if ((returnValue == 1) && (strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2)) == 0) )
	{
	//printf ("-->%s",tmpStr);
	//printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
	number = tmpStr[strlen(tmpStr)-3] - '0';
	//Ascii character '0'-'9' (0x30-0x39)
	//to interger(0)
	break;
	}
	}
	fclose(fp);
	if (returnValue == 1){
		sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
	}
	
	return returnValue;
}

void buttonThFunc(void)
{
	
	int readSize,inputIndex;
	struct input_event stEvent;
	while(1)
	{
	readSize = read(fd, &stEvent , sizeof(stEvent));
	if (readSize != sizeof(stEvent))
	{
	continue;
	}
	
	A.keyInput = stEvent.code;
	A.type = stEvent.type;
	A.pressed = stEvent.value;
	
	msgsnd(msgID,&A, sizeof(unsigned short) * 2 + sizeof(int) , 0);
	}

}

int buttonInit(void){
	
	if (probeButtonPath(buttonPath) == 0)
	return 0;
	fd=open (buttonPath, O_RDONLY);
	
	return 1;
}

int buttonStart(void){
	msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
	if (msgID == -1){	
		printf ("Cannot get msgQueueID, Return!\r\n");
		return -1;
	}
	A.messageNum = 1;
	pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL);
	
	return 0;
}

int buttonExit(void)
{
	pthread_join(buttonTh_id, NULL);
	
	close(fd);

	return 1;
}
