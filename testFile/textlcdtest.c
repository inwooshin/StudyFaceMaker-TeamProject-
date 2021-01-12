
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "textlcd.h"

int main(int argc, char * argv[]){
	lcdtestwrite(argv[1], argv[2], argc);
	
	return 0;
}
