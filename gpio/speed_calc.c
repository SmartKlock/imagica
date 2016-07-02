#include<stdio.h>
#include<wiringPi.h>
#define pinNumber 1
#define PPR 1
int main(void)
{
	int data=1,data1=1,time,time1,count=0;
	float speed;
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
		}
		if(count==30)
		{
			time=millis();
			time1=time-time1;
			speed=1800000.0/(PPR*(float)time1);
			printf("count changed to=%f, after %d milliseconds\n",speed,time1);
			time1=time;
			count=0;
		}
//		printf("value =%d\n",data);
		data1=data;
		delay(2);
	}
}
