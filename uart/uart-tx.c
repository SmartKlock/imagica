#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>


int main()
{
	int fd;
	int count;
	char data,data1[256];
	unsigned int nextTime;
//	setvbuf(stdin,(char*)NULL,_IONBF,0);

	printf("Testing this thing1\n");
	if ((fd =serialOpen("/dev/ttyAMA0",9600))<0)
	{
		fprintf(stdout, "unable to oopen serial device: %s\n",strerror(errno));
		return 1;
	}
	printf("Testing this thing2\n");
	if(wiringPiSetup() == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1;
	}
	printf("Testing this thing3\n");
	scanf("%s",&data1);
	sprintf(data1,"%s\n",data1);
	printf("Sending %s\n",data1);
//	sprintf(data1,"%c\n",data);
	serialPrintf(fd,"%s",data1);
//	fflush(fd);
	sleep(1);
	serialClose(fd);
	/**/
	return 0;
}
