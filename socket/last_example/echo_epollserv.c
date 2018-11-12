//2015118653
//양동현 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	struct epoll_event *ep_events;
	//epoll_event 인스턴스 
	struct epoll_event event;
	//epoll_event 인스턴스
	int epfd, event_cnt;
	//epoll 활용 변수 선언 

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

	epfd=epoll_create(EPOLL_SIZE);
	//epoll을 EPOLL_SIZE만큼 만들어서 epoll파일디스크립터를 epfd변수에 저장 
	ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
	//ep_event는 동적할당 해야한다. 

	event.events=EPOLLIN;
	//수신할 데이터가 존재하는 상황 발생시 이벤트유형 셋 
	event.data.fd=serv_sock;	
	//서버 소켓을 대상으로 
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	//epoll 인스턴스 생성 후 이곳에 관찰대상이 되는 파일디스크립터 등록 
	while(1)
	{
		event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		//epoll 파일디스크립터를 올리고 ep_event의 상황 발생시
		//이벤트가 발생한 fd수를 반환한다. 
		if(event_cnt==-1)
		{
			puts("epoll_wait() error");
			break;
		}

		for(i=0; i<event_cnt; i++)
		{
			//이벤트가 발생한만큼 반복한다. 

			if(ep_events[i].data.fd==serv_sock)
			{
				//이벤트가 발생한게 서버 소켓 파일 디스크립터일 경우 실행영역 
				adr_sz=sizeof(clnt_adr);
				clnt_sock=
					accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				//클라이언트와 accept 
				event.events=EPOLLIN;
				//클라이언트의 이벤트 상황 등록 
				event.data.fd=clnt_sock;
				//event에 클라이언트 소켓 파일 디스크립터 등록 
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				//epoll에 등록한다. 
				printf("connected client: %d \n", clnt_sock);
			}
			else
			{
					str_len=read(ep_events[i].data.fd, buf, BUF_SIZE);
					//이벤트가 발생한 i째 파일디스크립터로부터 읽어서 buf에 저장 
					if(str_len==0)    
					{
						//클라이언트 종료 요청시
						//클라이언트를 epoll에서 관리 하지 않게 한다. 
						epoll_ctl(
							epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);
						printf("closed client: %d \n", ep_events[i].data.fd);
					}
					else
					{
						write(ep_events[i].data.fd, buf, str_len);  
						//종료 요청이아닐시 클라이언트로 buf내용을 전송(쓰기)
					}
	
			}
		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
