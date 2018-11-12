#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc,char* argv[])
{
	int serv_sock,clnt_sock;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_size;

	char message[]="Hello world!";

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	//serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[1]));

	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
	{
		puts("socket error");
		exit(1);
	}

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
	{
		puts("bind error");
		exit(2);
	}

	if(listen(serv_sock,5)==-1)
	{
		puts("listen error");
		exit(3);
	}

	clnt_adr_size=sizeof(clnt_adr);
	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_size);
	if(clnt_sock==-1)
	{
		puts("accept error");
		exit(4);
	}
	write(clnt_sock,message,sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

	
