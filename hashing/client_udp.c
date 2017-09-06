#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define TEXT_SIZE 1024
#define IP1 "10.0.2.15"
#define IP2 "127.0.0.1"
#define PORT 5000



void main()
{
	short fd;
	struct sockaddr_in c;
	char buf[100]=TEXT;

	if((fd=socket(AF_INET,SOCK_DGRAM,0)) == 0)
	{
		printf("Error in Socket!! \n\n");
		exit(-1);
	}

	c.sin_family=AF_INET;
	c.sin_addr.s_addr = inet_addr(IP);
	c.sin_port = htons(PORT);

	printf("\nEnter the data to be sent:");
	scanf("%[^\n]s",buf);

	if ((sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&c, sizeof(c) ))>0)
	   printf("Data Written Successfully \n");	
	close(fd);
}
