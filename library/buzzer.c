
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
char FrequencyPath[200];
char EnablePath[200];
int fdFre, fdEna;

const int musicScale[MAX_SCALE_STEP] ={
	262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};


int buzzerInit(void){
	
	DIR * dir_info = opendir(BUZZER_BASE_SYS_PATH);
	int ifNotFound = 1;

	if (dir_info != NULL){

	while (1){
			struct dirent *dir_entry;
			dir_entry = readdir (dir_info);
			if (dir_entry == NULL) break;
			if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0){
				ifNotFound = 0;
				sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH,dir_entry->d_name);
			}
		}
	}
	else return ifNotFound;
	
	sprintf(EnablePath,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
	sprintf(FrequencyPath,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
	
	fdEna=open(EnablePath,O_WRONLY);
	fdFre=open(FrequencyPath,O_WRONLY);
	write(fdEna, &"0", 1);
	
	return ifNotFound;
}


int buzzerPlaySong(int scale){
	
	write(fdEna, &"1", 1);
	dprintf(fdFre, "%d", scale);
	
	return 0;
}

int buzzerStopSong(void){

	write(fdEna, &"0", 1);

	return 0;
}

int buzzerExit(void){
	close(fdFre);
	close(fdEna);

	return 0;
};
