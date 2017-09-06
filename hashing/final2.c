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
#define MAX 50

struct packet{
		int source_ip,dest_ip,source_port,dest_port;
                char data[256];
                time_t clk;
}*spr[100];
struct node{
                char data;
                struct node *next;
        };

struct node* add_end(struct node *,int key);


void insert_packet(struct packet *,struct packet *spr[], int ,int ,int ,int ,char []);
int addition_digits(int , int ,int , int);
int addition(long );
void update_packet(struct packet *spr[],int );
void lookup(struct packet *spr[],int ,int ,int ,int,char [] );



int main()
{
        int server_sockfd,client_sockfd,vara,serverfd1,clientfd1,result,maxfd,ret,source_bin;
int dest_bin,source1,dest1,server_len,client_len,slen,res,s_port,d_port;	
        struct sockaddr_in server_address,my_addr,client_address,  cli_addr;	
        fd_set readfds,testfds;
	unsigned char ch[256],ip[256];
struct packet spr[MAX]={ };
	
	struct packet *ptr;
	ptr=(struct packet *)malloc(sizeof(struct packet));
        if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)	
		perror("TCP error\n");
	else
		printf("TCP sucessfull\n");

	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr("192.168.131.33");
	server_address.sin_port=htons(9878);
	server_len=sizeof(server_address);
	client_len=sizeof(client_address);



	if((bind(server_sockfd,(struct sockaddr *)&server_address,server_len))==-1)	
		perror("Bind error\n");
	else
		printf("Bind sucessfull\n");

	listen(server_sockfd,5);

	if ((serverfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		perror("socket error\n");
	else
	      printf("udp successful\n");

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(9878);
	my_addr.sin_addr.s_addr = inet_addr("192.168.131.33");
	slen=sizeof(my_addr);

	if (bind(serverfd1, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1)
	      printf("bind error\n");
	
	while(1)
	{
		printf("\n");
		printf("server waiting\n");
		FD_ZERO(&readfds);		
		FD_SET(serverfd1,&readfds);
		FD_SET(server_sockfd,&readfds);
		if(serverfd1>server_sockfd)
			maxfd=serverfd1;
		else
			maxfd=server_sockfd;
		result=select(maxfd+1, &readfds, NULL, NULL, NULL);
		if(ret==-1)
		{
			perror("select");
			return -1;
		}
		if(FD_ISSET(server_sockfd,&readfds))
		{
			printf("receiving Tcp packet\n");
			client_sockfd=accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);
			dest_bin=server_address.sin_addr.s_addr;
			source_bin=client_address.sin_addr.s_addr;

			getsockname(server_sockfd,(struct sockaddr *)&server_address,&server_len);
			getpeername(client_sockfd,(struct sockaddr *)&client_address,&client_len);

			strcpy(ip,inet_ntoa(server_address.sin_addr));
			printf(" destination ip = %s\n",ip);
			s_port=htons(server_address.sin_port);
			printf("destination port=%d\n",s_port);

			strcpy(ip,inet_ntoa(client_address.sin_addr));
			printf("source ip=%s\n",ip);
			d_port=htons(client_address.sin_port);
			printf("source port=%d\n",d_port);

			read(client_sockfd,ch,sizeof(ch));
			printf("data recived from tcp client:%s\n",ch);
			printf("\n");
			insert_packet(ptr,*spr[100],source_bin,dest_bin,s_port,d_port,ch);
			lookup(*spr[100],source_bin,dest_bin,s_port,d_port,ip);
		}
		if(FD_ISSET(serverfd1,&readfds))
		{
			int len;
			len=sizeof(cli_addr);
			printf("receving udp packet\n");
			if (recvfrom(serverfd1,ch, 256, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
            			printf("received packet from");

			dest1=my_addr.sin_addr.s_addr;
			source1=cli_addr.sin_addr.s_addr;
			d_port=htons(my_addr.sin_port);
			s_port=htons(cli_addr.sin_port);

			getsockname(serverfd1,(struct sockaddr *)&my_addr,&slen);
			strcpy(ip,inet_ntoa(my_addr.sin_addr));
			printf("destination ip =%s\n",ip);
			printf("destination port no=%d\n",d_port);

			getpeername(serverfd1,(struct sockaddr *)&cli_addr,&len);
			strcpy(ip,inet_ntoa(cli_addr.sin_addr));
			printf("source ip=%s\n",ip);
			printf("source port no=%d\n",s_port);

			printf("data received from udp client :%s\n",ch);
			insert_packet(ptr,*spr[100],source_bin,dest_bin,s_port,d_port,ch);
			lookup(*spr[100],source_bin,dest_bin,s_port,d_port,ip);
		}
	}
		close(serverfd1);
		close(server_sockfd);

}

void insert_packet(struct packet *ptr,struct packet *spr[], int source_bin,int dest_bin,int s_port,int d_port,char ch[]) 
{
	 int count,key;
	unsigned char ip_str[256];
	int i,index,j;
	key=addition_digits(source_bin,dest_bin,s_port,d_port);
	index=key%MAX;
	printf("index:%d\n",index);
	
	if(source_bin==ptr->source_ip && dest_bin==ptr->dest_ip && s_port==ptr->source_port && d_port==ptr->dest_port)
	{
		count++;
		update_packet(spr,index);
	}
	else	
	{
		for(i=0;i<MAX;i++)
		{
			if(spr[index]->source_ip!=0)
			{
				index++;
				printf("updated index=%d\n",index);
			}
			spr[index]->source_ip=source_bin;
			spr[index]->dest_ip=dest_bin;
			spr[index]->source_port=s_port;
			spr[index]->dest_port=d_port;
			strcpy(spr[index]->data,ch);
			spr[index]->clk=time(NULL);
			printf("timestamp=%s",ctime(&spr[index]->clk));
			count++;
			break;
		}
		if(index==(MAX-1))
		{
			index=0;
		}
		printf("total no of packets received %d\n",count);
	}
}
struct node* add_end(struct node *head,int key)
{
	struct node *temp,*terv;
	temp=(struct node *)malloc(sizeof(struct node));
	temp->next=NULL;
	temp->data=key;
	if(head==NULL)
	{
		head=temp;
		return(head);
	}
	else
	{
		terv=head;
		while(terv->next!=NULL)
		{
			terv=terv->next;
		}
		terv->next=temp;
		return(head);
	}
}	


int add(int source,int dest,int sport,int dport)

{
int i;
i=dport^sport^source^dest*17;
return i;
}











void update_packet(struct packet *spr[],int index)
{
	spr[index]->clk=time(NULL);
	printf(" timestamp=%s",ctime(&spr[index]->clk));
	
}

void lookup(struct packet *spr[],int source_bin,int dest_bin,int s_port,int d_port,char ip[])
{
	int index;
	int var;
	char Ip_source[256];
	printf("enter 1 to lookup else 2 \n");
	scanf("%d",&var);
	if(var==1)
	{
		printf("enter the source ip address\n");
		scanf("%s",Ip_source);
		gets();
		if(strcmp(Ip_source,ip)==0)
		{
			index=addition_digits(source_bin,dest_bin,s_port,d_port)%MAX;
			printf("packet with source address %s found on index %d and data is %s\n",ip,index,spr[index]->data);
		}
	}
	else
		return;

}




