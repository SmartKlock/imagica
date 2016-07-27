#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>


int main()
{
	int fd;
	int count;
	char data[256];
	printf("Testing this thin1\n");
	if ((fd =serialOpen("/dev/ttyAMA0",9600))<0)
	{
		fprintf(stderr, "unable to oopen serial device: %s\n",strerror(errno));
		return 1;
	}
	printf("Testing this thin1\n");
	if(wiringPiSetup() == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1;
	}
	printf("Testing this thin1\n");
	while(serialDataAvail(fd)==0);
	printf("received-->");
	while(serialDataAvail(fd)>0)
	{
		printf("%c",serialGetchar(fd));
	}
	serialClose(fd);
	return 0;
}
