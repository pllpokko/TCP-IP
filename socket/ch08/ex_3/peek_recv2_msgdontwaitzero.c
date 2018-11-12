#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char *argv[])
{
	int acpt_sock, recv_sock;
	struct sockaddr_in acpt_adr, recv_adr;
	int str_len, state;
	socklen_t recv_adr_sz;
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	acpt_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&acpt_adr, 0, sizeof(acpt_adr));
	acpt_adr.sin_family=AF_INET;
	acpt_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	acpt_adr.sin_port=htons(atoi(argv[1]));
	
  	if(bind(acpt_sock, (struct sockaddr*)&acpt_adr, sizeof(acpt_adr))==-1)
		error_handling("bind() error");
	listen(acpt_sock, 5);
	
	recv_adr_sz=sizeof(recv_adr);
	recv_sock=accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);
	
	while(1)
	{
		str_len=recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK);
		//recv는 파일디스크립터로 부터 읽어온다. (read와 거의 같다)
		//recv_sock 파일디스크립터로 부터 읽어서 buf에 저장하고 (buf 사이즈 -1)만큼
		//옵션은 MSG_PEEK와 MSG_DONTWAIT를 준다.
		//MSG_PEEK는 입력버퍼에 수신 된 데이터가 존재하는지 확인하는용도
		//확인만 하므로 읽혀져도 입력버퍼에서 데이터가 지워지지 않는다.
		//MSG_DONTWAIT 옵션과 함께써서 블로킹 되지 않고 데이터 존재유무확인

		if(str_len>0)
			break;
	}

	buf[str_len]=0;
	printf("Buffering %d bytes: %s \n", str_len, buf);
 	
	str_len=recv(recv_sock, buf, sizeof(buf)-1, 0);
	//위의 rev의 설명과 동일하고 옵션은 0을 넣어서 옵션을 쓰지 않는다.
	
	buf[str_len]=0;
	printf("Read again: %s \n", buf);
	close(acpt_sock);
	close(recv_sock);
	return 0; 
}


