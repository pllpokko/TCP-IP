//2015118653
//양동현
//Yang Dong Heyon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h> //TCP_NODELAY를 위한 헤더파일 선언

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;
	
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	
	int opt_val; //옵션값을 담을 변수
	socklen_t opt_len;
	int state; //상태를 담을 변수

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	opt_len=sizeof(opt_val);
	if((state=getsockopt(serv_sock,IPPROTO_TCP,TCP_NODELAY,(void*)&opt_val,&opt_len))==-1)
	{
		//서버 소켓의 TCP_NODELAY 의 값을 읽어온다.
		//getsockopt 함수 실행이 실패하면 -1을 반환하는데 그때 에러 처리를 한다.
		printf("getsockopt Error");
		exit(2);
	}
	if(opt_val==0)
	{
		//getsockopt 함수를 사용해서 TCP_NODELAY 의 값은 opt_val 로 저장되는데
		//opt_val값이 0이면 Nagle Algorithm 이 적용되고 있다는 뜻이다.
		puts("This socket is operated on Nagle Algorithm. I will do Nagle OFF!");
	}
	opt_val=1;
	if((state=setsockopt(serv_sock,IPPROTO_TCP,TCP_NODELAY,(void*)&opt_val,sizeof(opt_val)))==-1)
	{
		//TCP_NODELAY에 setsockopt함수를 이용해 1을 넣는다. 1을 넣으면 nagle이 OFF 된다.
		puts("Nagle Off Fail");
	}
	else if(state==0)
	{
		//setsockopt함수가 제대로 작동하면 0을 반환한다.
		puts("Nagle Off Success");
	}



	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	if(listen(serv_sock, 1)==-1)
		error_handling("listen() error");
	
	clnt_adr_sz=sizeof(clnt_adr);

	

	for(i=0; i<1; i++)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i+1);
	
		while((str_len=read(clnt_sock, message, BUF_SIZE))!=0)
			write(clnt_sock, message, str_len);

		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
