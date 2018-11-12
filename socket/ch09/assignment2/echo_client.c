//2015118653
//양동현
//Yang Dong Hyeon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h> //TCP_NODELAY를 위한 헤더파일
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	int opt_val; //옵션 값을 담을 변수
	socklen_t opt_len;
	int state; //상태값 담을 변수 


	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	opt_len=sizeof(opt_val);
	if((state=getsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(void*)&opt_val,&opt_len))==-1)
	{
		//클라이언트 소켓의 TCP_NODELAY의 값을 읽어온다.
		//getsockopt 함수가 실행이 실패하면 Error를 띄운다.
		printf("getsockopt Error");
		exit(2);
	}
	if(opt_val==0)
	{
		//getsockopt 함수를 사용하여 TCP_NODELAY 값 읽어온다.
		//0이면 Nagle Algorithm이 사용중이다.
		puts("This socket is operated on Nagle Algorithm. I will do Nagle OFF!");
	}
	opt_val=1;
	if((state=setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(void*)&opt_val,sizeof(opt_val)))==-1)
	{

		//setsockopt 함수로 TCP_NODELAY에 1을 쓴다.
		//1을 쓰면 Nagle Algorithm을 끈다.
		//state의 반환값이 -1이면 setsockopt함수가 실패한 것이다.
		
		puts("Nagle Off Fail");
	}
	else if(state==0)
	{
		//함수가 성공적으로 실행되면 nalge이 off된다.
		puts("Nagle Off Success");
	}


	
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	while(1) 
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len=read(sock, message, BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server: %s", message);
	}
	
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
