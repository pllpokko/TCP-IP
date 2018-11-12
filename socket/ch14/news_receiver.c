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

int main(int argc, char *argv[])
{
	int recv_sock;
	int str_len;
	char buf[BUF_SIZE];
	struct sockaddr_in adr;
	struct ip_mreq join_adr; //멀티캐스트 그룹의 주소정보와 가입할 호스트 주소정보를
	//담을 구조체를 인스턴스한다. 
	
	if(argc!=3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	 }
  
	recv_sock=socket(PF_INET, SOCK_DGRAM, 0); //멀티캐스트 리시버도 UDP기반 소켓이다. 
 	memset(&adr, 0, sizeof(adr));
	adr.sin_family=AF_INET;
	adr.sin_addr.s_addr=htonl(INADDR_ANY);	
	adr.sin_port=htons(atoi(argv[2]));
	
	if(bind(recv_sock, (struct sockaddr*) &adr, sizeof(adr))==-1)
		error_handling("bind() error");
	
	join_adr.imr_multiaddr.s_addr=inet_addr(argv[1]); //multicast용 IP를 담는다. 
	join_adr.imr_interface.s_addr=htonl(INADDR_ANY); //가입할 호스트 주소 정보를 담는다.
  	

	//소켓 옵션을 줘서 호스트를 멀티캐스트에 가입시킨다.
	//IPPROTO_IP의 IP_ADD_MEMBERSHIP으로 가능하고
	//멀티캐스트 IP와 가입할 호스트 주소 정보를 담긴 구조체를 4번째 매개변수로 전달 
	setsockopt(recv_sock, IPPROTO_IP, 
		IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));
  
	while(1)
	{
		str_len=recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
		//BUF_SIZE-1 만큼 담아
		if(str_len<0) 
			break;
		buf[str_len]=0;
			//버프 맨마지막엔 널문자 넣고 
		fputs(buf, stdout);
		//화면으로 띄워준다. 
	}
	close(recv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
