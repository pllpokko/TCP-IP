//2015118653
//양동현 YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc, char *argv[])
{
	int i;
	struct hostent *host;
	struct sockaddr_in addr;
	if(argc!=2) {
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr=inet_addr(argv[1]);
	
	host=gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	//gethostbyaddr 함수는 IP주소를 받아서 도메인에 대한 정보를 반환한다.
	//매개변수로 IP주소,길이(IPv4 4Byte),IP주소 형태(AF_INET))을 전달
	//반환된 도메인 정보는 hosten 형 포인터구조체변수에 담는다.
	
	if(!host)
		error_handling("gethost... error");

	printf("Official name: %s \n", host->h_name);

	//IP주소에 대한 도메인네임 중 Official name를 출력한다.
	for(i=0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i+1, host->h_aliases[i]);
	

	//다른도메인 이름들도 출력한다.
	printf("Address type: %s \n", 
		(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
	//도메인의 IP주소 형태를 출력한다.
	for(i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1,
					inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));	
	//문자열 형태로 예를들면 74.122.15.186 
	//받은 IP주소를 출력한다.
	//ntoa 는 이렇게 숫자로 바꾼 IP주소를 문자열 출력할 수 있게 해준다.
	return 0;
}

