#include<stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN STDIN_FILENO
void main()
{
	struct timeval tv;
	fd_set  readfds;
	long timer=0,count=0;
	while(1)
	{
		tv.tv_sec=0;
		tv.tv_usec=5000;
		FD_ZERO(&readfds);
		FD_SET(STDIN, &readfds);
		select(STDIN+1,&readfds,NULL,NULL,&tv);
		if(FD_ISSET(STDIN, &readfds))
		{
			char data;
			scanf("%c",&data);//fgetc(STDIN);
			printf("key pressed %c\n",data);
			if(data=='C')
			{
				printf("Exiting\n");
				return;
			}
		}else
		{
			timer++;
			if(timer>20)
			{
				timer=0;
				printf("%d\n",count++);
			}
		}
	}
}
