#include<stdio.h>	
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<errno.h>
#define TCP 6
#define UDP 17
void insert( unsigned char dest_ip[15],  short port,unsigned char curtime[], short protocol,unsigned char buf[]);
void diplay();
//void lookup(char dest_ip[15]);
struct hash
{	
	unsigned short port;
	unsigned char dest_ip[15];
	unsigned char time[30];
	unsigned short protocol;
	unsigned short count;
	unsigned char data[50];
	struct hash *next;

}*h[100];
short hash(char *dest_ip)
{
	short i;
	short sum=0;
	for(i=0;(dest_ip[i])!='\0';i++)
	   if ((dest_ip[i])>='0' && (dest_ip[i])<='9')
		sum= sum + (short)( (dest_ip[i])-48);
	
	   

	return (sum % 100) ;
	
}


void main(void)
{
	struct sockaddr_in s_fd,c_fd;
	unsigned short tcp;
	unsigned short udp;
	unsigned short cfd;
	unsigned short client_socket[50] , max_clients = 90 ;
	unsigned short sd, max_sd;
	unsigned short activity, i;
	unsigned char buf[2048], *temp, *htime;
	unsigned char c_name[INET_ADDRSTRLEN];
	unsigned char s_name[INET_ADDRSTRLEN];
	time_t current_time;
	fd_set fds;
	socklen_t len= sizeof(c_fd);

	if((tcp=socket(AF_INET,SOCK_STREAM,0))==0)
	{
		printf("Error in create TCP Socket\n\n");
		exit(EXIT_FAILURE);
	}

	if((udp=socket(AF_INET,SOCK_DGRAM,0))==0)
	{
		printf("Error in create UDP Socket\n\n");
		exit(EXIT_FAILURE);
	}	

	s_fd.sin_family=AF_INET;
	s_fd.sin_addr.s_addr=INADDR_ANY;
	s_fd.sin_port=htons(8985);

	if((bind(tcp,(struct sockaddr *)&s_fd, sizeof(s_fd)))<0)
	{
		printf("Error in TCP Bind\n\n");
		exit(EXIT_FAILURE);
	}

	if((bind(udp,(struct sockaddr *)&s_fd, sizeof(s_fd)))<0)
	{
		printf("Error in UDP Bind\n\n");
		exit(EXIT_FAILURE);
	}

	if((listen(tcp,5))<0)
	{
		printf("\nError in Server Listen\n\n");
		exit(EXIT_FAILURE);
	}

	printf("\nWaiting for connections..!!\n");		

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(tcp,&fds);
		FD_SET(udp,&fds);		

		//add child sockets to set
       		for ( i = 0 ; i < max_clients ; i++) 
        	{

			sd = client_socket[i];
			if(sd > 0)
			FD_SET( sd , &fds);
			if(sd > max_sd)
			max_sd = sd;
		}

		activity = select( max_sd + 1 , &fds , NULL , NULL , NULL);
		if ((activity < 0) && (errno!=EINTR))  
		{  
			printf("select error");  
		}
		
		if(FD_ISSET(tcp,&fds))
		{
			if((cfd=accept(tcp,(struct sockaddr *)&c_fd, &len))<0)
			{
				printf("ERROR in Accept!!\n\n");
				exit(-1);
			}
			
					

			read(cfd, buf, 2048);
			inet_ntop(AF_INET,&c_fd.sin_addr.s_addr, c_name,sizeof(c_name));

			time(&current_time);
			htime=ctime(&current_time);

			insert(c_name, ntohs(c_fd.sin_port), htime, TCP, buf);

			display();
			
		}// End of IF

		else if(FD_ISSET(udp,&fds))
		{
			if((recvfrom(udp, buf, sizeof(buf), 0, (struct sockaddr *)&c_fd, &len))<0)
				printf("\n\nError reading data!!");

			inet_ntop(AF_INET,&c_fd.sin_addr.s_addr, c_name,sizeof(c_name));

			time(&current_time);
			htime=ctime(&current_time);


			insert(c_name, ntohs(c_fd.sin_port), htime, UDP, buf);

			display();
			
		}// End of ELSE IF 

close(cfd);		
	}	//End of while(1)
close(tcp);
close(udp);

}	//End of main()


void insert(unsigned char dest_ip[15],  short port,unsigned char curtime[], short protocol,unsigned char buf[])
{
	char index,c,i,cmp;
	struct hash *temp;
	index= hash(dest_ip);

	if(h[index]==NULL)
	{
		h[index] = (struct hash *) malloc( sizeof(struct hash) );

		if(h[index]==NULL)
		{
			printf("failed\n");
			exit(-1);
		}
		
		temp = h[index];
		if(protocol == TCP)
		   temp->protocol = TCP;
		else if(protocol == UDP)
		   temp->protocol = UDP;
		else
		{
			printf("\n\nothers");
			exit(-1);
		}

		// memset(char *dest_ip,4, sizeof(buf));
		for(c=0;dest_ip[c]!='\0';c++)
	   	   temp->dest_ip[c]=dest_ip[c];
		temp->dest_ip[c]='\0';

		temp->port=port;

		for(c=0;curtime[c]!='\0';c++)	
		   temp->time[c]=curtime[c];

		temp->next=NULL;

		++(temp->count);
		
	}

	else
	{
	  temp = h[index];

	  while(temp)
	  {
		if((cmp = strcmp(temp->dest_ip,dest_ip))==0)
		{
		if(protocol == TCP)
		   temp->protocol = TCP;
		else if(protocol == UDP)
		   temp->protocol = UDP;
		else
		{
			printf("\n\other");
			exit(-1);
		}
			

			for(c=0;curtime[c]!='\0';c++)	
		  	 temp->time[c]=curtime[c];

			++(temp->count);
			temp->port=port;

			for(i=0;buf[i]!='\0';i++)
			   temp->data[i]=buf[i]; 
			   temp->data[i]='\0';

			break;

		}
		
		else if(temp->next==NULL)
			break;

		else 
		temp=temp->next;

	  } // end of while

		if(temp->next==NULL && cmp!=0) 
		{
			temp->next = (struct hash *) malloc( sizeof(struct hash) );

			if(temp->next ==NULL)
			{
				printf("failed");
				exit(-1);
			}		

			temp=temp->next;
			if(protocol == TCP)
		   temp->protocol = TCP;
		else if(protocol == UDP)
		   temp->protocol = UDP;
		else
		{
			printf("\n other");
			exit(-1);
		}
			
			
			for(c=0;dest_ip[c]!='\0';c++)
		   	   temp->dest_ip[c]=dest_ip[c];

			temp->dest_ip[c]='\0';
			temp->port=port;

			for(c=0;curtime[c]!='\0';c++)	
		  	 temp->time[c]=curtime[c];

			temp->next=NULL;
			++(temp->count);

			for(i=0;buf[i]!='\0';i++)
			   temp->data[i]=buf[i];
			   temp->data[i]='\0';
		}	//end of if

	}	//end of else 
}
void display()
{
	unsigned short i;
	unsigned short count;
	struct hash *temp;

	printf("table");
	printf("\n\n");
	for(i=0;i<100;i++)
	{
	   	count=0;
		temp=h[i];
	   	
	   	if(temp==NULL)
	     	   continue;
	   	else
	   	{
		   printf("\n\nINDEX (%d) ", i);
		   while(temp!=NULL)
		   {		
			if(temp->protocol == TCP)
			{
				printf("ip address: %s\n",temp->dest_ip);
				printf("port number is %d\n",temp->port);
				printf("time at which packet arrived is %s\n",temp->time);
				printf("data is %d\n",temp->data);
				printf("count is %d\n",temp->count);
			}
			if(temp->protocol == UDP)
			{
				printf("ip address: %s\n",temp->dest_ip);
				printf("port number is %d\n",temp->port);
				printf("time at which packet arrived is %s\n",temp->time);
				printf("data is %d\n",temp->data);
				printf("count is %d\n",temp->count);
			}
			temp=temp->next;
		   }

	   	}    

	} 

}
void srch_sprt(char dest_ip)
{
	struct node *temp;
	int c=1,s,i,n;
	unsigned char ip;
	printf("Enter the ip to search.\n");
	scanf("%d",&ip);
	for(i=0;i<100;i++)
	{
		if(ip[i]!=NULL)
		{
			temp=ip[i];
			while(temp!=NULL)
			{
				if(temp->data==s)
				{
					c++;
					n++;
					break;
				}
				temp=temp->next;
				n++;
			}
			if(c!=1)
			{
				printf("ip %d found at %d node of %d index.\n",s,n,i);
				break;
			}
		}
	}
	if(c==1)
	{
		printf("ip not found.\n");
	}
	return;
}




