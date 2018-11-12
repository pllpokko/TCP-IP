//2015118653
//YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);  
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	pid=fork();//포크한다
	//자식 프로세스의 pid값은 0
	if(pid==0)//자식프로세스 실행영역 
		write_routine(sock, buf);//자식프로세스는 전송 루틴 실행 
	else 
		read_routine(sock, buf);//부모 프로세스는 수신 루틴 실행 

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	while(1)//무한루프
	{
		int str_len=read(sock, buf, BUF_SIZE);
		//sock 파일 디스크럽터로 부터 읽어서 buf에 쓴다.
		//문자 길이를 리턴해서 str_len에 저장한다. 
		if(str_len==0)//문자를 0개 받으면 read_routine함수를 종료한다. 
			return;

		buf[str_len]=0;//buf에 받은 문자열 맨 끝에 Null문자 삽입 
		printf("Message from server: %s", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while(1)//무한루프 
	{
		fgets(buf, BUF_SIZE, stdin);//키보드로 부터(stdin) 
		//입력을 받아들여서 buf에 저장한다. 
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{	
			shutdown(sock, SHUT_WR);//q만 입력되면 소켓을 끈다. 
			//shutdown 함수는 TCP연결 종료 절차를 실시한다. 
			//sock 소켓 연결을 종료 한다.
			//send buffer에 남아있는 데이터는 모두 내보낸뒤 TCP연결종료 
			return;//wrtie_routine 함수도 종료한다. 
		}
		write(sock, buf, strlen(buf));//일반적인 경우 그냥 전송한다. 
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
