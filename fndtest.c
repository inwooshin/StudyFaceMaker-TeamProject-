#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "fnd.h"

int main(int argc, char* argv[]){
	int number;
	
	number = error(argc, argv);
	if(number == -1) return 0;
	
	fnd(number, *argv[1]);
}
