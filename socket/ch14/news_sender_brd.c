//201511863
//YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in broad_adr;
	FILE *fp;
	char buf[BUF_SIZE];
	int so_brd=1;
	//브로드캐스트를 1로 set할 변수 
	if(argc!=3) {
		printf("Usage : %s <Boradcast IP> <PORT>\n", argv[0]);
		exit(1);
	}
  
	send_sock=socket(PF_INET, SOCK_DGRAM, 0);	
	memset(&broad_adr, 0, sizeof(broad_adr));
	broad_adr.sin_family=AF_INET;
	broad_adr.sin_addr.s_addr=inet_addr(argv[1]); //브로드캐스트 전송할 네트워크IP주소 
	broad_adr.sin_port=htons(atoi(argv[2])); //포트번호 
	
	//브로드캐스트용 소켓 옵션 설정 
	//위에서 set한 브로드 캐스트를 켠다. 
	setsockopt(send_sock, SOL_SOCKET, 
		SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));	
	if((fp=fopen("news.txt", "r"))==NULL)
		error_handling("fopen() error");
	//news.txt를 읽어온다. 

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 
			0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
		//end of file 포인터까지 계속 전송 
		sleep(2);
		//한번 전송하고 2초 블럭 시킨다. 
	}

	close(send_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
