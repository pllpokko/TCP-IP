//2015118653
//양동현 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	//인자로 전달된 fd_set형 변수를 0으로 초기화 
	FD_SET(serv_sock, &reads);
	//fd_set형 변수 reads에 serv_sock에 해당하는 배열을 셋한다. 
	//즉 serv_sock 소켓을 관리대상으로 올린다. 
	fd_max=serv_sock;
	//serv_sock까지 등록됬음으로 fd_max에 serv_sock번호까지 파일디스크립터 저장 

	while(1)
	{
		cpy_reads=reads;
		//reads는 원본으로 두고 cpy_reads를 사용한다. 
		timeout.tv_sec=5;
		timeout.tv_usec=5000;
		//select 함수호출이후 무한블로킹 상태 빠지지말라고 
		//몇초간 타임아웃설정을 하기 위한 인자 

		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
			break;
		//select 함수. 서버소켓 파일디스크립터까지 관리하고 관리대상 참조는 cpy_reads를 참조한다.
		//3,4번째 매개변수는 각각 write관련/ 예외상황관련인데 안쓰니까 0.
		//time아웃은 &timeout을 참조하여 블로킹 방지
		
		
		if(fd_num==0)
			continue;
		//이벤트가 바뀌지 않았으면 0을 반환하기 때문에
		//0이면 다시 반복문시작 
		for(i=0; i<fd_max+1; i++)
		{
			//반복문을 돌면서 체크해야한다. 
			if(FD_ISSET(i, &cpy_reads))
			{
				//cpy_reads에서 이벤트가 발생해서 셋된거 있고 
				//그 셋이 serv_sock이라면 
				if(i==serv_sock)     
				{
					adr_sz=sizeof(clnt_adr);
					clnt_sock=
						accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					//서버와 클라이언트를 accept하고 
					FD_SET(clnt_sock, &reads);
					//reads에 방금받은 클라이언트 소켓을 등록한다. 
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					//관리대상을 하나 올린다. 
					printf("connected client: %d \n", clnt_sock);
				}
				else    
				{
					str_len=read(i, buf, BUF_SIZE);
					//i에 해당하는 파일 디스크립터가 변화가 있는데
					//serv_sock이 아닌경우에 그 파일디스크립터로부터 내용을 읽어서 buf에 저장

					if(str_len==0)    
					{
						//str_len==0이면 클라이언트에서 종료요청 
						FD_CLR(i, &reads);
						//그러므로 현재 클라이언트 소켓 파일디스크립터를
						//관리대상에서 제외 
						close(i);
						printf("closed client: %d \n", i);
					}
					else
					{
						write(i, buf, str_len);  
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
