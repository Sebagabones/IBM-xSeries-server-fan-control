#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <getopt.h>

#define	OPTLIST		"vh"

bool verbose = false;

int temps(int numCores)
{
	FILE *fp;
	char path[1035];
	int coreTemps[numCores];
	fp = popen("/usr/bin/sensors", "r");
	if(fp == NULL) {
		exit(1);
	}
	int counter = 0;
	while(fgets(path, sizeof(path), fp) != NULL)
	{
		char* temp = strtok(path, " ");
		if(!strcmp(temp,"Core"))
		{
			temp = strtok(NULL, " ");
			temp = strtok(NULL, " ");
			char temps[10];
			memcpy(temps, &temp[1], 4);
			temps[4] = '\0';
			coreTemps[counter] = atoi(temps);
			counter += 1;
		}

	}
	int check = 0;
	for(int i = 0; i < numCores; ++i)
	{
		if(verbose)
		{
			printf("Core %i (out of %i)is %i\n", i, numCores, coreTemps[i]);
		}
		if(coreTemps[i] >= 65)
		{
			check = 2;
			break;
		}
		if(coreTemps[i] >= 55  && check != 2)
		{
			check = 1;
		}
	}
	if(check == 2)
	{
		system("sudo ipmitool raw 0x3a 0x07 0x01 0xff 0x01 && sudo ipmitool raw 0x3a 0x07 0x02 0xff 0x01");
	}
	else if(check == 1)
	{
		system("sudo ipmitool raw 0x3a 0x07 0x01 0xff 0x00 &&  sudo ipmitool raw 0x3a 0x07 0x02 0xff 0x00");
	}
	else
	{
		system("sudo ipmitool raw 0x3a 0x07 0x02 0x34 0x01 && sudo ipmitool raw 0x3a 0x07 0x01 0x34 0x01");
	}
	pclose(fp);
	return check;
}


int main(int argc, char * argv[])
{
//	int numberOfCores = (atoi(argv[1]));
	int numberOfCores = (get_nprocs() / 2);
	int temp = temps(numberOfCores);

	return 0;
}
