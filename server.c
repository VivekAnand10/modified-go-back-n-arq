#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<unistd.h>
//creating the buffer where the packet will be stored.
char a[256];
char b[10];
char c[10];
void main(int argc,char **argv)
{
	//creating a socket for server.
	int ser_socket,client_addr,status,count,count2,size,i=0,m=0,n=0;
	ser_socket=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser_address,client;
	ser_address.sin_family=AF_INET;
	ser_address.sin_port=atoi(argv[2]);
	ser_address.sin_addr.s_addr=inet_addr(argv[1]);
	//binding the incoming socket connection with the server socket.
	status=bind(ser_socket,(struct sockaddr *)&ser_address,sizeof(ser_address));
	if(status==-1)
	{
		printf("\n---unable to bind---\n");
		return;
	}
	listen(ser_socket,1);
	strcpy(c,"Timeout");
	int client_socket;
	//accepts the socket connection coming from client.
	client_socket=accept(ser_socket,(struct sockaddr *)&client,&client_addr);
	//creating a file pointer.
	FILE *fptr;
	//opening the file.
	fptr=fopen(argv[3],"w");
	//error checking.
	if(fptr==NULL)
	{
		printf("\n---error opening file----\n");
	}
	while(1)
	{
		//receiving the packet coming from the client.
		count=recv(client_socket,a,256,0);
		if(strcmp(a,c)==0)
		{
			break;
		}
		//writing the the data of incoming packet into the file.
		fwrite(a,1,255,fptr);
		//error checking.
		if(count==-1)
		{
			printf("\n---error1---\n");
		}
		else
		{
			printf("\n----RECEIVE PACKET %d LENGTH %d LAST PKT",n++,256);
		}
		//converting the acknowledgement number to a string.
		sprintf(b,"%d",m);
		//sending the acknowledgement to the client.
		count2=send(client_socket,b,10,0);
		if(count2==-1)
		{
			printf("\n---error2---\n");
		}
		printf("\n----SEND ACK %d",m);
		m++;
	}
	//closing the connection.
	printf("\n----CLOSING CONNECTION");
	close(ser_socket);
}

