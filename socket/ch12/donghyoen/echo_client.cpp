#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#define BUF_SIZE 1024
void error_handling(char *message);
void read_routine(int sock,char *buf);
void write_routine(int sock,char *buf);

using namespace std;
int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;



	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	
	//modified mode
	char temp[]="modified";
	write(sock,temp,strlen(temp));


	printf("5");
	printf("\nID :");
	char ID[21];
	scanf("%s",ID);
	getchar();

	write(sock,ID,sizeof(ID));
	//ID전송 

printf("6");


	printf("Year : ");
	char year[21];
	scanf("%s",&year);
	getchar();
	
	write(sock,year,sizeof(year));

	printf("Month : ");
	char month[21];
	scanf("%s",&month);
	getchar();

	printf("4");
	write(sock,month,sizeof(month));


	FILE* readfp=fdopen(sock,"r");
	FILE* writefp=fdopen(sock,"w");
	char temp3[21];
	fgets(temp3,sizeof(temp3),readfp);
	int tt=atoi(temp3);
	printf("3");
	
	if(tt==-1)//-1은 ID를 새로 생성하는 경우
	{
		while(1)
		{

		printf("First Login ID\n");
		printf("Please set a new PASSWORD : ");
		char temp1[21];
		scanf("%s",temp1);
		getchar();
		printf("Please enter it again : ");
		char temp2[21];
		scanf("%s",temp2);
		getchar();
	
			if(!strcmp(temp1,temp2))
			{
	
				printf("Your new account creation is complete\n");
				sprintf(temp1,"%s%c",temp1,'\n');
				fputs(temp1,writefp);
				fflush(writefp);
				break;
			}
		}



	}

	//처음 파일을 쫙 받아옴//

	FILE* readfp;
	readfp=fdopen(sock,"r");
	while(1)
	{
		if(fgets(buf,sizeof(buf),readfp)==NULL)
			break;
		fputs(buf,stdout);
		fflush(stdout);
	}


	pid=fork();
	if(pid==0)
		write_routine(sock,buf);
	else
		read_routine(sock,buf);
	


	close(sock);
	return 0;
}
void read_routine(int sock, char *buf)
{
	while(1)
	{
		int str_len=read(sock, buf, BUF_SIZE);
		if(str_len==0)
			return;

		buf[str_len]=0;
		if(buf[0]=='*')//달력용 데이터
		{

		}
		printf("Message from server: %s", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while(1)
	{
		fgets(buf, BUF_SIZE, stdin);
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{	
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));
	}
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
