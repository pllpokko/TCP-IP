#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char* message);
int main(int argc,char* argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;

	struct sockaddr_in serv_adr;

	if(argc!=3)
	{
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	char *str[3]={"Hello server!","I'm client","Nice to meet you too!"};
	int i_size[3];
	for(int i=0;i<3;i++)
	{
		i_size[i]=strlen(str[i]);
	}

	for(int i=0;i<sizeof(str)/sizeof(char);i++)
	{
		int recv_len;
		read(sock,&recv_len,sizeof(recv_len));
		int recv_cnt=0;
		while(recv_cnt<recv_len)
		{
			int temp;
			temp=read(sock,&message[recv_cnt],BUF_SIZE-1);
			if(temp==-1)
				error_handling("read from serv error!");
			recv_cnt+=temp;
		}
		message[recv_len]=0;
		printf("%s\n",message);

		write(sock,&i_size[i],sizeof(int));
		write(sock,str[i],i_size[i]);

	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	printf("%s\n",message);
	exit(1);
}

	
