#include<stdio.h>
#include<time.h>

void main()
{
	FILE *test;//=fopen("/sys/class/gpio/gpio21/value","r");
	int i;
	char data='A';
		printf("opening file\n");
	test=fopen("/sys/class/gpio/gpio21/value","r");
	for(i=0;i<5000;i++)
	{
		printf("retriving data\n");
//		test=fopen("/sys/class/gpio/gpio21/value","r");
		data=fgetc(test);
		rewind(test);
		printf("sensor value=%c\n",data);
//		printf("\nclosing file\n");
//		fclose(test);
		sleep(1);
	}
		fclose(test);
}
