//2015118653
//YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64 //TTL 사이즈 결정 
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in mul_adr;
	int time_live=TTL; //TTL 설정용 변수 지금 define에서 TTL은 64이다. 
	FILE *fp;
	char buf[BUF_SIZE];

	if(argc!=3){
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	}
  	
	send_sock=socket(PF_INET, SOCK_DGRAM, 0); //UDP 기반 소켓 만든다. 
	memset(&mul_adr, 0, sizeof(mul_adr));
	mul_adr.sin_family=AF_INET;
	mul_adr.sin_addr.s_addr=inet_addr(argv[1]);  // 멀티 캐스트용 IP주소
	mul_adr.sin_port=htons(atoi(argv[2]));       // 멀티 캐스트용 포트 번호 
	
	//소켓 옵션 설정 
	//send socket을 TTL 설정한다. 
	//IPPROTO_IP에서 IP_MULTICAST_TTL로 설정한다.
	//4번째 매개변수에 TTL 사이즈를 담은 변수의 주소를 void 포인터로 전달 
	setsockopt(send_sock, IPPROTO_IP, 
		IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
	
	if((fp=fopen("news.txt", "r"))==NULL)
		error_handling("fopen() error");

	//파일 포인터 fp를 news.txt 읽기전용으로 부른다. 
	while(!feof(fp))   
	{
		//fp가 end of file 되기 전까지 계속 루프 문 
		fgets(buf, BUF_SIZE, fp);
		//버프에 담는다. 
		//30Byte씩 담은걸 sendto로 전송 (방송) 한다. 
		sendto(send_sock, buf, strlen(buf), 
			0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));
		sleep(2);
		//2초 블럭 
	}
	fclose(fp);
	close(send_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
