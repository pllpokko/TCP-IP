#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
int main(int argc,char* argv[])
{
	char message[BUF_SIZE];
	int serv_sock;
	struct sockaddr_in serv_addr,from_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(atoi(argv[1]));

	serv_sock=socket(PF_INET,SOCK_DGRAM,0);


	int str_len;
	socklen_t from_addr_size;
	while(1)
	{
		printf("send message : ");
		fgets(message,BUF_SIZE,stdin);
		if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
			break;
		str_len=sendto(serv_sock,message,strlen(BUF_SIZE),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
		from_addr_size=sizeof(from_addr);
		recvfrom(serv_sock,message,str_len,0,(struct sockaddr*)&from_addr,&from_addr_size);
	}
	close(serv_sock);
	return 0;
}

