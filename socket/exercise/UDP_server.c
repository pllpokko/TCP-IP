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
	int str_len;
	int serv_sock;
	struct sockaddr_in serv_addr,clnt_addr;

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_sock=socket(PF_INET,SOCK_DGRAM,0);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	
	socklen_t clnt_addr_size;
	while(1)
	{
		clnt_addr_size=sizeof(clnt_addr);
		recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
		sendto(serv_sock,message,strlen(message),0,(struct sockaddr*)&clnt_addr,clnt_addr_size);
	}
	close(serv_sock);

}
