#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <getopt.h>

#define	OPTLIST		"dvh"

bool verbose = false;
bool dryRun = false;

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
	if(!dryRun)
	{
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
			system("sudo ipmitool raw 0x3a 0x07 0x02 0x27 0x01 && sudo ipmitool raw 0x3a 0x07 0x01 0x27 0x01");
		}
	}
	pclose(fp);
	return check;
}


int main(int argc, char * argv[])
{
    int  opt;
    opterr = 0;
    while((opt = getopt(argc, argv, OPTLIST)) != -1)   
    {
//  verbose flag option
        if(opt == 'v')
        {
                verbose = true;
                printf("verbose output\n");
        }
		else if(opt == 'h')
		{
			printf("Run with -v for verbose, or -d for a dry run - otherwise give no arguments\n");
			exit(0);
		}
		else if(opt == 'd')
		{
			dryRun = true;
			verbose = true;
		}
	}
//	int numberOfCores = (atoi(argv[1]));
	int numberOfCores = (get_nprocs() / 2);
	if(verbose)
	{
		printf("number of cores is %i\n", numberOfCores);
	}
	int temp = temps(numberOfCores);
	if(verbose)
	{
		if(dryRun)
		{
			printf("Dry run, although I would have run: ");
		}
		else
		{
			printf("Ran ");
		}
		if(temp == 0)
		{
			printf("'sudo ipmitool raw 0x3a 0x07 0x02 0x27 0x01 && sudo ipmitool raw 0x3a 0x07 0x01 0x27 0x01'\n");
			printf("(lowest fan speed)\n");
		}
		else if(temp == 1)
		{
			printf("'sudo ipmitool raw 0x3a 0x07 0x01 0xff 0x00 &&  sudo ipmitool raw 0x3a 0x07 0x02 0xff 0x00'\n");
			printf("(medium fan speed)\n");
		}
		else if(temp == 0)
		{
			printf("'sudo ipmitool raw 0x3a 0x07 0x01 0xff 0x01 && sudo ipmitool raw 0x3a 0x07 0x02 0xff 0x01'\n");
			printf("(highest fan speed)\n");
		}
	}
	return 0;
}
