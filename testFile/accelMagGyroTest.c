#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "acc.h"

int main (void)
{
	accInit();
	magInit();
	gyroInit();
	
	getAcc();
	getMag();
	getGyro();
	
	return 0;
}
