//2015118653
//양동현 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
	FILE * readfp; //readfp 파일포인터 
	FILE * writefp; //write fp 파일포인터 

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	readfp=fdopen(sock, "r"); //fdopen으로 sock(파일디스크립터)를 파일포인터에서
	//쓸수 있게한다. 
	writefp=fdopen(sock, "w");	

	while(1) 
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;
		//표준입력에서 입력받은 데이터를 message에 저장
		//q나 Q가 입력되면 나간다. 
		fputs(message, writefp);
		//wrtiefp에 meesage내용을 입력한다. 
		fflush(writefp);
		//writefp의 버퍼를 비운다. 
 		fgets(message, BUF_SIZE, readfp);
		//readfp에 있는 내용을 meesage에 기록한다. 
		printf("Message from server: %s", message);
		//메세지를 출력한다. 
	}	
	fclose(writefp);
	fclose(readfp);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
