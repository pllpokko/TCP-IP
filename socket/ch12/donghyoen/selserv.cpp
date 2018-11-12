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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	FILE *fin;
	int serv_sock, clnt_sock; //소켓 디스크립터 만듬
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout; //타임아웃 구조체 선언
	fd_set reads, cpy_reads; //fd_set형 변수 두개를 만든다. 하나는 copy본이다.
	int fd;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	//fd_max는 검사할 파일디스크립터 갯수를 넣는다.
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

	FD_ZERO(&reads);//reads의 안의 파일디스크립터를 전부 0으로 만든다. 즉 초기화한다.
	FD_SET(serv_sock, &reads);//serv_sock에는 파일디스크립터 번호가 있는데
	//reads에서 serv_sock에 맞는 파일디스크립터 번호(위치)에 1로 셋한다.
	//즉 서버 소켓에 변화가 있는지 관심을 두고 지켜본다는 말이다.
	fd_max=serv_sock;
	//검사할 파일디스크립터 갯수에 serv_sock을 넣는다.

	while(1)
	{
		cpy_reads=reads; //cpy_reads에 reads를 넣고 앞으로 select할 때 copy본을 쓴다.
		//이유는 select 함수 호출이 끝나면 변화가 생긴 파일 디스크립터의 위치를 제외한 나머지 위치의 비트들은
		//0으로 초기화 된다. 따라서 원본의 유지를 위해서 복사본을 사용한다.
		timeout.tv_sec=5;
		timeout.tv_usec=5000;
		//select 함수호출 전에 타임아웃값을 설정한다.
		//select 함수 호출 후에는 구조체 timeval의 멤버 tv_sec와 tv_usec에 저장된 값이 타임아웃이 발생하기 까지
		//남았던 시간으로 바뀌기 때문이다. 따라서 select 함수를 호출하기 전에 매번 timeval 구조체 변수의 초기화를
		//반복해야한다. 
		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
			break;
		//select함수를 호출한다.
		//첫 번째 매개변수는 최대 검사할 파일디스크립터 갯수이다. 0~fd_max 까지 즉 (fd_max+1)개 검사한다.
		//fd_max에는 serv_sock이 들어갔으므로 0~서버 소켓파일디스크립터까지 검사한다.

		//두 번째 매개변수는 fd_set* readset이다.
		//fd_set형 변수에 '수신된 데이터의 존재여부'에 관심이 있는 파일 디스크립터 정보를 모두 등록해서 그 변수의
		//주소 값을 전달한다.

		//세 번째 매개변수는 fd_set* writeset이다.
		//fd_set형 변수에 '블로킹 없는 데이터 전송의 가능여부'에 관심 있는 파일 디스크립터 정보를 모두 등록해서
		//그 변수의 주소 값을 전달한다.

		//네 번째 매개변수는 fd_set *exceptset이다.
		//fd_set형 변수에 '예외상황의 발생여부'에 관심이 있는 파일 디스크립터 정보를 모두 등록해서
		//그 변수의 주소 값을 전달한다.

		//다섯 번째 매개변수는 select 함수 호출 이후에 무한정 블로킹 상태에 빠지지 않도록 타임아웃을 설정하기 위한 인자
		//를 전한다.

		//반환값은 오류 발생시에 -1, 타임 아웃에 의한 반환 시에는 0, 관심 대상으로 등록된 파일 디스크립터에 해당하는
		//관심 디스크립터에 변화가 발생하면 변화가 발생한 파일 디스크립터의 수를 반환한다.


		
		if(fd_num==0)
			continue;
		//타임 아웃이 발생한 경우 fd_num에 0이 들어가는데 그러면 continue 실행

		for(i=0; i<fd_max+1; i++)//fd_num이 1이상 반환되었을 때 실행하는 반복문이다.
		{ 
			//0~파일디스크립터까지 체크한다.
			
			if(FD_ISSET(i, &cpy_reads))//cpy_reads에서 i번째 파일디스크립터가 셋되있다면 1이 반환된다.
			{//수신된 데이터가 있는 파일 디스크립터를 찾는다.
				if(i==serv_sock)     
				{
					//상태변화가 확인이 되면 제일 먼저 서버 소켓에서 변화가 있었는지 확인한다.
					//서버 소켓의 상태변화가 맞다면
					adr_sz=sizeof(clnt_adr);
					clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					//클라이언트와 accept 수락을 하자.
					
					FD_SET(clnt_sock, &reads);
					if (fd_max<clnt_sock)
						fd_max = clnt_sock;
					//clnt_sock을 reads에 그에 맞는 파일디스크립터에 1로 셋한다.
					//앞으로 reads의 clnt_sock위치에 1이 되있으므로 이 클라이언트에
					//요청이 있을 경우  아래 else문을 실행한다.
					
					printf("connected client: %d \n", clnt_sock);
				}
				else    // read message!
				{
					//상태변화가 발생한 소켓이 서버 소켓이 아닌 경우에 실행된다.
					//즉 수신할 데이터가 있는 경우에 실행된다.
					str_len=read(i, buf, BUF_SIZE);
					buf[str_len]=0;
					if(str_len==0)   
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
						//st_len이 0이면 연결을 끝냄으로
						//FD_CLR로 i(clnt sock 파일디스크립터)를 0으로 만든다.
						//그리고 clnt를 종료한다.
					}
					else
					{


						//modified모드
						if(!strcmp("modified",buf))
						{
							char ID[21];
							str_len=read(i,ID,sizeof(ID));
							ID[str_len]=0;
							strcat(ID,".txt");
							fin=fopen(ID,"r");
							if(fin==NULL){
								fd = open(ID, O_CREAT|O_RDWR|O_APPEND);
								fin = fdopen(fd,"r");
							}
							while(!feof(fin))
							{
								char str[4][500];
								fscanf(fin,"%s",str[0]);
								fscanf(fin,"%s",str[1]);
								fscanf(fin,"%s",str[2]);
								fscanf(fin,"%s",str[3]);
								//fscanf(fin,"%s",str[4]);
								strcpy(buf,"*");
								strcat(buf,str[0]);
								strcat(buf,str[1]);
								strcat(buf,str[2]);
								strcat(buf,"\n");
								//char *result;
								//if((result=fgets(buf,BUF_SIZE,fin))!=NULL);
								//strcat(result,"\n");
								str_len=write(i,buf,sizeof(buf));
							}
							fclose(fin);
							char year[21];
							str_len=read(i,year,sizeof(buf));
							year[str_len]=0;

							char month[21];
							str_len=read(i,month,sizeof(buf));
							month[str_len]=0;
							
							//for debug
							printf("ID : %s\n",ID);
							printf("year : %s\n",year);
							printf("month : %s\n",month);
							
							//버퍼 비우기

							




						}

						else{
						for(int i=4;i<fd_max+1;i++)
						{
							write(i,buf,str_len);
						}
						}
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
