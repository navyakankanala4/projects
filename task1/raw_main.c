#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //memset
#include<netinet/if_ether.h>
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#define TCP 6
#define UDP 17
char addr[INET_ADDRSTRLEN];
int tcp_count=0;
int udp_count=0;
int sock_raw;


struct sockaddr_in source,dest;
 

//details for hash function
struct hash_parameters
{	
	unsigned short src_port;
	unsigned char dest_ip[15];
	unsigned char src_ip[];
	unsigned short protocol;
	unsigned short dest_port;
	unsigned short count;
	
}*h[10];
struct hash_table
{
int key;
unsigned char time[50];
struct hash_table *next;
struct hash_parameters *req;
}*arr[30];
short hash(char *dest_ip)
{
	short i;
	short sum=0;
	for(i=0;(dest_ip[i])!='\0';i++)
	   if ((dest_ip[i])>='0' && (dest_ip[i])<='9')
		sum= sum + (short)( (dest_ip[i])-48);
	
	   

	return (sum % 100) ;
	
}
time_t current_time;
void insert(unsigned int key,struct hash_parameters *temp,char *ctime);
void display();
void get_ethernet_ip(char *buffer,int data_size);
int main()
{
    int saddr_size , data_size;
	
    struct sockaddr saddr;
saddr_size=(sizeof(saddr));
    struct in_addr in ;
	source.sin_family=AF_INET;
	source.sin_addr.s_addr=inet_addr("192.168.131.33");
	source.sin_port=htons(9732);
    unsigned char *buffer = (unsigned char *)malloc(65536); 
    memset(buffer,0,65536);
    //Create a raw socket that shall sniff
    sock_raw = socket(AF_PACKET , SOCK_RAW , htons(ETH_P_ALL));
    printf("socket created succesffully\n");
    if(sock_raw < 0)
    {
        printf("Socket Error\n");
        return 1;
    }
    while(1)
    {
	printf("server waiting\n");
         printf("packets started \n");
        //Receive a packet
      int data_size=recvfrom(sock_raw,buffer,65536,0,&saddr,&saddr_size);
        if(data_size <0 )
        {
            printf("Recvfrom error , failed to get packets\n");
            return -1;
        }
      get_ethernet_ip(buffer,data_size);
	 ;
index=hash(dest_ip);
key=index%100;
time(&current_time);
			htime=ctime(&current_time);
insert(key,struct hash_parameters *temp,char time);
	//print_ethernet_ip();
	close(sock_raw);
    	printf("Finished");
    	return 0;

 
}
}
void get_ethernet_ip(char *buffer,int data_size)
{

unsigned int iphdrlen;
//To construct the Ethernet header, fill all the fields of the ethhdr structure:
struct ethhdr *eth = (struct ethhdr *)(buffer);//ethernet header
printf("\nEthernet Header\n");
struct ip *ip = (struct ip*)(buffer + sizeof(struct ethhdr));
struct iphdr *iph = (struct iphdr*)(buffer);//Get the IP Header part of this packet
iphdrlen=iph->ihl*4;
//if the protocol is tcp
if(iph->protocol==TCP)
{
struct tcphdr *tcp = (struct tcphdr *)(buffer + sizeof(struct ip));
struct tcp_info *tcp_info=(struct tcp_info*)(buffer+sizeof(struct ip));
memset(&addr,0, INET_ADDRSTRLEN);//macro is to convert to binary
strcpy(addr,inet_ntoa(ip->ip_src));
printf("source ip %s",addr);
memset(&addr,0, INET_ADDRSTRLEN);
strcpy(addr,inet_ntoa(ip->ip_dst));
printf("destination ip %s",addr);
printf("source port is %d\n",ntohs(tcp->th_sport));
printf("destination port is %d\n",ntohs(tcp->th_dport));
tcp_count++;
printf("no of packets sent %d\n",tcp_count);
}
//if the protocol is udp
else if(iph->protocol==UDP)
{
struct udphdr *udp = (struct udphdr *)(buffer + sizeof(struct ip));
//struct udp_info *udp_info=(struct udp_info*)(buffer+sizeof(struct ip));
memset(&addr,0, INET_ADDRSTRLEN);//macro is to convert to binary
strcpy(addr,inet_ntoa(ip->ip_src));
printf("source ip %s",addr);
memset(&addr,0, INET_ADDRSTRLEN);
strcpy(addr,inet_ntoa(ip->ip_dst));
printf("destination ip %s",addr);
printf("source port is %d\n",ntohs(udp->udph_sport));
printf("destination port is %d\n",ntohs(udp->udph_dport));
udp_count++;
printf("no of udp packets sent is %d\n",udp_count);
}

}



//insert and display functions
void insert(unsigned key , struct hash_parameters *temp , char *c_time)
{

	unsigned index = (key % 100 );
	
	if(arr[index] == NULL) 
	{
		arr[index] = (struct table *)malloc(sizeof(struct hash_table));
		arr[index]->key = key ;
		strcpy(arr[index]->time , c_time ) ;
		arr[index]->req= temp ;
		arr[index]->next =NULL ;
		
	}
	
	else
	{
		struct hash_table *tp = arr[index] ;
		while(tp->next == NULL)
		{
			if(tp->key == key)
			{
				strcpy(tp->time , c_time) ;
				return ;
			}
			tp = tp->next ;

		}
		tp=tp->next ;
		tp =(struct table *)malloc(sizeof(struct hash_table)) ;
		if(tp == NULL)
		{
			printf("malloc returned null\n");
			exit(0);
		}
		tp->key = key;
		strcpy(tp->time,c_time);
		tp->req = temp;
		tp->next = NULL ;
		return ;
	}
}



void display(unsigned int key)
{
	unsigned index = key %100 ;
	if(arr[index] == NULL)
	{
		printf("No record stored with this key\n") ;
		return ;
	}
	
	struct sockaddr_in sa;
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (struct in_addr *)&(arr[index]->req->source_ip), str, INET_ADDRSTRLEN);

	
	printf("1.time %s\n",arr[index]->time);
	printf("2.source ip is %s\n",str);
	return ;
}




    
