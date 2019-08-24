#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
typedef struct data_packet
{
	char packet_type;
	char *data;
	int packet_seq_no;
}packet;
int W;
char b[10];
char c[10];
int frame_nos(int packet_size,char *p)
{
	FILE *fp;
	int i=0,x=0;
	fp=fopen(p,"r");
	if(fp==NULL)
	{
		printf("---error reading the file ---");
	}
	else
	{
		while(getc(fp)!=EOF)
		{
			i++;
		}
		x=i/packet_size;
	}
	printf("total no of frames=%d\n",x);
	return x;
}
void main(int argc,char **argv)
{
	//taking the window size from the command line arguments.
	W=atoi(argv[5]);
	int sock,status,framenos,packet_size,count1,count2,k,f_no,c_sent=0,p,wl,i;
	//taking the packet size from the command line argument and changing it to be an integer.
	packet_size=atoi(argv[4]);
	//creating a socket for the client.
	sock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serveraddr;
	//assignment the ip address and the port no to the respecting field of the structure of client.
	//type of protocol.
	serveraddr.sin_family=AF_INET;
	//port number.
	serveraddr.sin_port=atoi(argv[2]);
	//ipaddress.
	serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
	packet pac;
	//dynamically assigning the size to the packet created.
	pac.data=malloc(packet_size);
	//finding out the total number of frames.
	f_no=frame_nos(packet_size,argv[3]);
	//connecting to the server and checking its status.
	status=connect(sock,(struct sockaddr *) &serveraddr,sizeof(serveraddr));
	strcpy(c,"Timeout");
	//error handling.
	if(status==-1)
	{
		printf("----there was an error while connecting---\n");
	}
	else
	{
		printf("-----TCP connection established\n");
	}
	//opening the file whose data is to sent to the server.
	FILE *fp;
	fp=fopen(argv[3],"r");
	if(fp==NULL)
	{
		printf("\n----error reading file----\n");
	}
	//GO BACK N ARQ implementation.
	while(1)
	{
		for(i=0;i<W;i++)
		{
			//creating a packet.
			while((pac.data[k]=fgetc(fp))!=EOF && k!=packet_size-2)
			{
				k++;
			}
			pac.data[++k]='\0';
			//sending the packet.
			count1=send(sock,pac.data,256,0);
			//error checking.
			if(count1==-1)
			{
				printf("\n-----error---\n");
			}
			k=0;
			if(c_sent>=f_no)
			{
				break;
			}
			if(c_sent<f_no)
			{
				printf("\n----SEND PACKET %d",c_sent);
			}
			c_sent++;
		}
		i=0;
		wl=W;
		while(i<W)
		{
			//receiving the acknowledgement.
			count2=recv(sock,b,10,0);
			p=atoi(b);
			if(count2==-1)
			{
				printf("----error----");
			}
			i++;
			if(p<f_no)
			{
				printf("\n----RECEIVE ACK %d",p);
				wl--;
			}
			else
			{
				break;
			}
			if(p>=f_no)
			{
				break;
			}
		}
		if((c_sent)>=f_no)
		{
			count1=send(sock,c,10,0);
			break;
		}
		//checking whether timeout has occured.
		if(strcmp(b,c)==0)
		{
			break;
		}
	}
	//closing the connection
	printf("\n----CLOSING CONNECTION\n");
	close(sock);
}
