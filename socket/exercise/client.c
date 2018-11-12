#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc,char* argv[])
{
	char message[BUFSIZ];
	int serv_sock;
	struct sockaddr_in serv_addr;

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(atoi(argv[1]));

	serv_sock=socket(PF_INET,SOCK_STREAM,0);

	bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	socklen_t clnt_addr_size;
	connect(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	int str_len,recv_len,recv_cnt;
	while(1)
	{
		memset(message,0,sizeof(message));
		printf("send message : ");
		fgets(message,BUFSIZ,stdin);
		if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
			break;
		str_len=write(serv_sock,message,strlen(message));
		recv_len=0;
		recv_cnt=0;
		while(recv_len<str_len)
		{
			recv_cnt=read(serv_sock,&message[recv_len],BUFSIZ);
			if(recv_cnt==-1)
				exit(1);
			recv_len+=recv_cnt;
		}
		message[recv_len]=0;
		printf("%s",message);
	}
	close(serv_sock);

	return 0;
}

