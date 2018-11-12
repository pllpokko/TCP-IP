//2015118653
//양동현 YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
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
	struct hostent *host;//gethostbyname()함수의 출력을 담기위한 구조체
	//이곳에 도메인네임에 해당하는 정보들이 들어간다.
	if(argc!=2) {
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}
	
	host=gethostbyname(argv[1]);//argv[1]로 받아들인 도메인 주소를
	//gethostbyname()함수를 이용해 정보를 가져오자.host에 담는다.
	if(!host)
		error_handling("gethost... error");

	printf("Official name: %s \n", host->h_name);
	//공식 도메인네임을 출력한다.
	for(i=0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i+1, host->h_aliases[i]);
	//같은 메인 페이지 인데도 다른 도메인 이름으로 접속 할 수 있는 경우가 
	//있는데 하나의 ip에 둘이상의 도메인 네임을 지정할수 있다.
	//이들 정보는 h_aliases를 통해서 얻을 수 있다.

	printf("Address type: %s \n", 
		(host->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");
	//type도 알 수 있는데 h_addrtype 속에 IPv4인지 IPv6인지 정보가 들어간다.
	for(i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d: %s \n", i+1,
					inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	//제일 중요한 도메인이름에 해당하는 Ip주소가 들어가는 곳이다.
	//하나의 도메인 이름에 대응하는 IP를 여러 개를 만들어
	//둘이상의 서버로 둘 수있는데 이러한 경우들 때문에
	//IP주소가 여러개 올 수있다.
	//여러개의 IP주소들이 h_addr_list배열속에 담겨져있다.
	return 0;
}


