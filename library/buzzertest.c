
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "buzzer.h"

#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨
const int musicScale[MAX_SCALE_STEP] ={
	262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};

int main(int argc , char **argv){
	
	int freIndex;
	
	if (argc < 2 ){
		printf("Error!\n");
		printf("Usage:\n");
		printf("buzzertest <buzzerNo> \n");
		printf("buzzerNo: \n");
		printf("do(0),re(1),mi(2),fa(3),sol(4),ra(5),si(6),do(7) \n");
		printf("stop(-1)\n");
		return 1;
	}
	
	freIndex = atoi(argv[1]);
	printf("freIndex :%d \n",freIndex);
	
	if ( freIndex > MAX_SCALE_STEP-1 ){
		printf(" <buzzerNo> over range \n");
		return 1;
	}
	
	buzzerInit();
	
	if ( freIndex == -1){
		buzzerStopSong();
		return 0;
	}
	
	buzzerPlaySong(musicScale[freIndex]);
	
	buzzerExit();
	
	return 0;
}
