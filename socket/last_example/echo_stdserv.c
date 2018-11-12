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
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;
	
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	FILE * readfp; //읽기용 파일 포인터 
	FILE * writefp; //쓰기용 파일 포인터 
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_adr_sz=sizeof(clnt_adr);

	for(i=0; i<5; i++)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i+1);
	
		readfp=fdopen(clnt_sock, "r");
		//클라이언트소켓(파일디스크립터)를 파일포인터로 쓸 수 있게 
		//fdopen으로 readfd에 줘서 파일포인터로 사용한다.
		writefp=fdopen(clnt_sock, "w");
		//클라이언트소켓(파일디스크립터)를 파일포인터로 쓸 수 있게 
		//fdopen으로 wirtefp에 줘서 파일포인터로 사용한다. 
		while(!feof(readfp))
		{
			//readfp 파일포인터가 파일 엔드 오프 파일까지 반복한다. 
			fgets(message, BUF_SIZE, readfp);
			//readfp 파일포인터로부터 읽어서 message에 BUF_SIZE만큼 쓴다. 
			fputs(message, writefp);
			//write fp파일포이넡에 message를 쓴다. 
			fflush(writefp);
			//writefp로 writefp의 버퍼를 비워준다.
			//표준입출력에서는 버퍼가 생기는데
			//이를 비워주지 않으면 당장에 클라이언트로 데이터 전송을 보장하지 못한다. 
		}
		fclose(readfp);
		fclose(writefp);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
