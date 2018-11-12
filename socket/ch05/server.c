#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int main(int argc,char *argv[])
{
	int serv_sock,clnt_sock;
	char message[BUF_SIZE];
	int str_len,i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;

	socklen_t clnt_adr_sz;

	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET,SOCK_STREAM,0);
	if(serv_sock==-1)
		error_handling("socket() error");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock,1)==-1)
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);

	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
	if(clnt_sock==-1)
		error_handling("accept() error");
	
	char *str[3]={"Hello client!","I'm server.","Nice to meet you."};
	int i_size[3];
	for(int i=0;i<3;i++)
	{
		i_size[i]=strlen(str[i]);
	}


	for(int i=0;i<sizeof(str)/sizeof(char);i++)
	{
		write(clnt_sock,&i_size[i],sizeof(int)); 
		write(clnt_sock,str[i],i_size[i]);
		int recv_len;
		read(clnt_sock,&recv_len,sizeof(recv_len));
		int recv_cnt=0;
		while(recv_cnt<recv_len)
		{
			int temp;
			temp=read(clnt_sock,&message[recv_cnt],BUF_SIZE-1);
			if(temp==-1)
				error_handling("read form clnt error!");
			recv_cnt+=temp;
		}
		message[recv_len]=0;
		printf("%s\n",message);
			
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char* message)
{
	printf("%s\n",message);
	exit(1);
}
