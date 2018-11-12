//2015118653
//YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int fds[2];
	
	pid_t pid;
	struct sigaction act;//SIGNAL을 처리할 용도 
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	//자식 프로세스가 종료되면 시그널을 울린다 
	//그리고 act에 저장된 함수를 실행한다. 

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	pipe(fds);//fds[0],fds[1]을 PIPE연결 시킨다.
	pid=fork();//포크 
	if(pid==0)
	{
		FILE * fp=fopen("echomsg.dat", "wt");
		//자식프로세스는 echomsg.dat 이라는 파일을 
		//wt 모드 즉 쓰기전용 및 텍스트 파일 이라는 옵션으로
		//연다. 
		char msgbuf[BUF_SIZE];
		int i, len;

		for(i=0; i<10; i++)
		{
			len=read(fds[0], msgbuf, BUF_SIZE);
			/*
			   여기 자식 프로세스는 PIPE를 통해 들어온 fds[0]메세지를 읽어서
			   msgbuf에 저장하고 fwrite로 fp포인터대상 텍스트파일로
			   내용을 저장하는 일을 10번한다.
			   */
			fwrite((void*)msgbuf, 1, len, fp);
			//msgbuf를 len 길이만큼 fp파일로 저장한다. 
		}
		fclose(fp);
		return 0;
		//10회 for문을 반복하면 fp를 닫고 0을 운영체제에 올리고 자식 프로세스 종료 
	}

	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			puts("new client connected...");

		pid=fork();
		if(pid==0)
		{
			//또 자식 프로세스 영역 
			close(serv_sock);
			//서버 프로세스를 종료한다 
			while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0)
			{
				//클라이언트 소켓으로 읽어서 buf 에 저장한다.

				write(clnt_sock, buf, str_len);
				//클라이언트 소켓으로 내용을 그대로 전송한다. 
				write(fds[1], buf, str_len);
				//buf 내용을 PIPE통신을 이용 (fds[1])해서 fds[0]으로 보내게된다. 
			}
			
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
