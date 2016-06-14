#include<stdio.h>
#include<wiringPi.h>
#define pinNumber 29

int main(void)
{
	int data=1,data1=1,time,time1,count=0;
	if(wiringPiSetup() == -1)
	{
		printf("Wiring pi is a dud\n");
		return 1;
	}
	pinMode( pinNumber , INPUT);
	time=millis();
	time1=time;
	for(;;)
	{
		data=digitalRead(pinNumber);
		if(data != data1 && data ==0)
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
