#include<stdio.h>
#include<wiringPi.h>
#define pinNumber 1

int main(int argc,char** argv)
{
	int data=1,data1=1,time,time1,count=0,pin=pinNumber;
	if(wiringPiSetup() == -1)
	{
		printf("Wiring pi is a dud\n");
		return 1;
	}
	if(argc>1)
	{
		sscanf(argv[1],"%d",&pin);
	}
	pinMode( pin , INPUT);
	time=millis();
	time1=time;
	for(;;)
	{
		data=digitalRead(pin);
		if((data != data1) & (data !=0))
		{
			count++;
			time=millis();
			time1=time-time1;
			printf("count changed to=%d, after %d milliseconds\n",count,time1);
			time1=time;
		}
//		printf("value =%d\n",data);
		data1=data;
		delay(2);
	}
}
