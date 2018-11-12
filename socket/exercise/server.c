#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc,char* argv[])
{
	char message[BUFSIZ];
	int serv_sock,clnt_sock;
	struct sockaddr_in serv_addr,clnt_addr;
	
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	
	bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	listen(serv_sock,5);
	int str_len;
	socklen_t clnt_addr_size;
	for(int i=0;i<5;i++)
	{
		clnt_addr_size=sizeof(clnt_addr);
		clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_sock,&clnt_addr_size);

		while((str_len=read(clnt_sock,message,BUFSIZ))!=0)
			write(clnt_sock,message,BUFSIZ);
		close(clnt_sock);
	}
	close(serv_sock);

	return 0;
}
