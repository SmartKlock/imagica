#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/time.h>
#include <unistd.h>

#include<wiringPi.h>

#define PinNumber 29

#define MAXCLIENTS	10
#define WHEEL_PPR	10
#define WHEEL_DIA	550

#define SEND_SPEEDa




int threadcomplete=0;

struct ClientStatus{
	int IsConnected;
	pthread_t ThreadId;
	int ClientSocketFileDiscriptor,ClientLength;
	struct sockaddr_in ClientAddress;
	char Buffer[256];
};

void error(char *msg)
{
	printf(msg);
//	exit(1);
}



int main(int argc, char *argv[])
{
	struct ClientStatus Clients[MAXCLIENTS+1];
	int sockfd,newsockfd,portno,clilen,n,ClientCount=0,data[2],Count=0,time[3];
	FILE *ftime;
	char buffer[256];
	float speed=0;
	double distance=0,increment=3.14*WHEEL_DIA/WHEEL_PPR;
	struct sockaddr_in serv_addr, cli_addr;

	struct timeval tv;
	fd_set readfds,exceptfds;
	if(wiringPiSetup()==-1)
	{
		printf("wiring pi is a dud\n");
		return 1;
	}
	pinMode(PinNumber,INPUT);

	for(ClientCount=0;ClientCount<MAXCLIENTS+1;ClientCount++)
	{
		Clients[ClientCount].IsConnected=0;
	}

	if(argc < 2)
	{
		fprintf(stderr,"error, no port provided\n");
		error("Error, no port provided\n");
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket\n");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port=htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
	{
		error("Error on binding\n");
	}
	listen(sockfd,5);
	data[0]=0;
	data[1]=0;
	time[0]=millis();
	time[1]=time[0];
	time[2]=time[0];
	ftime=fopen("log.txt","w");
	while(1)
	{
		int maxid=0;
		int printoutput=0;
		long currentTime=millis();
		if((currentTime-time[2])>50)
		{
			printoutput=1;
			time[2]=millis();
		}
		data[0]=digitalRead(PinNumber);
		if(data[0] !=data[1])
		{	
//			printf("printing data to file\n");
//			ftime=fopen("log.txt","w");
			fprintf(ftime,"%d\n",currentTime);
//			fclose(ftime);
			if( data[0]==0)
			{
				Count++;
				distance+=increment;
			}
		}
		data[1]=data[0];
		if(Count==30)
		{
			time[0]=millis();
			time[1]=time[0]-time[1];
			speed=(increment*0.03)/(3600.0*((float)time[1]));
			time[1]=time[0];
			Count=0;
		}
		tv.tv_sec=0;
		tv.tv_usec=100;
		FD_ZERO(&readfds);
//		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		FD_SET(0, &readfds);
		FD_SET(sockfd, &readfds);
		FD_SET(sockfd, &exceptfds);
		maxid=sockfd;
		for(ClientCount=0;ClientCount<10;ClientCount++)
		{
			if(Clients[ClientCount].IsConnected==1)
			{
				FD_SET(Clients[ClientCount].ClientSocketFileDiscriptor, &readfds);
//				FD_SET(Clients[ClientCount].ClientSocketFileDiscriptor, &writefds);
				FD_SET(Clients[ClientCount].ClientSocketFileDiscriptor, &exceptfds);
				if(Clients[ClientCount].ClientSocketFileDiscriptor>maxid)
				{
					maxid=Clients[ClientCount].ClientSocketFileDiscriptor;
				}
			}
		}
		select(maxid+1,&readfds,NULL,&exceptfds,&tv);
		if(FD_ISSET(0,&readfds))
		{
			char datac;
			scanf("%c",&datac);
			if(datac=='C')
			{
				printf("Closing Server\n");
				fclose(ftime);
				for(ClientCount=0;ClientCount<MAXCLIENTS;ClientCount++)
				{
					if(Clients[ClientCount].IsConnected==1)
					{
						close(Clients[ClientCount].ClientSocketFileDiscriptor);
					}
				}
				close(sockfd);
				return 0;
			}
		}
		if(FD_ISSET(sockfd,&readfds))
		{
			
//			printf("accepting Connection\n");
			for(ClientCount=0;ClientCount<MAXCLIENTS+1;ClientCount++)
			{
				if(Clients[ClientCount].IsConnected==0)
				{
					break;
				}else
				{
//					printf("Client point %d is busy %d\n",ClientCount,Clients[ClientCount].IsConnected);
				}
			}

//			printf("Accepting Client on %d\n",ClientCount);
			Clients[ClientCount].ClientLength = sizeof(Clients[ClientCount].ClientAddress);
			Clients[ClientCount].ClientSocketFileDiscriptor = accept(sockfd, (struct sockaddr *) &(Clients[ClientCount].ClientAddress),&(Clients[ClientCount].ClientLength));
			if(Clients[ClientCount].ClientSocketFileDiscriptor < 0)
			{
				printf("ERROR on accept\n");
			}else
			{
				if(ClientCount!=MAXCLIENTS)
				{
					Clients[ClientCount].IsConnected=1;
				}else
				{			
					printf("cannot Accomodate any more clients\n");
					close(Clients[ClientCount].ClientSocketFileDiscriptor);
				}
			}
		}
		
		for(ClientCount=0;ClientCount<MAXCLIENTS;ClientCount++)
		{	
			if((Clients[ClientCount].IsConnected==1)&&(FD_ISSET(Clients[ClientCount].ClientSocketFileDiscriptor,&exceptfds)))
			{
//				printf("Client Closed Connection on Client number %d\n",ClientCount);
				close(Clients[ClientCount].ClientSocketFileDiscriptor);
				Clients[ClientCount].IsConnected==0;
			}
			if((Clients[ClientCount].IsConnected==1)&&(FD_ISSET(Clients[ClientCount].ClientSocketFileDiscriptor,&readfds)))
			{
				int errorsoc = 0;
				socklen_t len = sizeof (errorsoc);
				int retval = getsockopt (Clients[ClientCount].ClientSocketFileDiscriptor
							, SOL_SOCKET
							, SO_ERROR
							, &errorsoc
							, &len);
				if((retval!=0)||(errorsoc!=0))
				{
					printf("Client Closed Connection on Client number %d\n",ClientCount);
					close(Clients[ClientCount].ClientSocketFileDiscriptor);
					Clients[ClientCount].IsConnected=0;
					continue;
				}
				bzero(buffer,256);
				n=read(Clients[ClientCount].ClientSocketFileDiscriptor,buffer,255);
				if ( n <0 )
				{
					printf("ERROR reading from socket\n");
				}
				printf("Here is the message received from cleint %d : %s\n",ClientCount,buffer);
			}
			if((Clients[ClientCount].IsConnected==1))//&&(FD_ISSET(Clients[ClientCount].ClientSocketFileDiscriptor,&writefds)))
			{
				bzero(buffer,256);
				if(printoutput==1)
				{
#ifdef SEND_SPEED
					sprintf(buffer,"%f %f\n",distance,speed);
#else
					sprintf(buffer,"%f\n",distance);
#endif
					n=-1;
					while(buffer[++n]!=0);
//					buffer[++n]='\n';
//					printf("%f\n",speed);
					n = write(Clients[ClientCount].ClientSocketFileDiscriptor, buffer,n+1);
					if(n<0)
					{
						error("ERROR writing to socket\n");
					}
				}
			}
		}
	}
	return 0;
}
