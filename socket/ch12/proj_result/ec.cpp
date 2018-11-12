#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#define BUF_SIZE 1024

int year, month;

void error_handling(char *message);
void read_routine(int sock, char *buf,int year, int month,int now_client_sock);
void write_routine(int sock, char *buf,int now_client_sock);
void read_file(int sock,char *buf);
void make_cal(int year, int month,int sock);
void itoa(int n, char s[]); 
void reverse(char s[]);

typedef struct
{
	int year;
	int month;
	int day;
}CAL;

using namespace std;
int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;



	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected...........");


	//modified mode
	char temp[] = "modified";
//	write(sock, temp, strlen(temp));


	printf("\nID :");
	char ID[21];
	scanf("%s", ID);
	getchar();

//	write(sock, ID, sizeof(ID));
	//ID?꾩넚 




	printf("Year : ");
	char year2[21];
	scanf("%s", year2);
	getchar();

//	write(sock, year2, sizeof(year2));

	printf("Month : ");
	char month2[21];
	scanf("%s", month2);
	getchar();

	fflush(stdin);

	int MySockNumber;
	str_len=read(sock,buf,sizeof(buf));
	buf[str_len]=0;
	int now_client_sock=atoi(buf);
//	printf("%d",atoi(buf));
//	write(sock, month2, sizeof(month2));


	
	year = atoi(year2);
	month = atoi(month2);
	make_cal(year,month,now_client_sock);

	pid = fork();
	if (pid == 0)
		write_routine(sock, buf,now_client_sock);
	else
		read_routine(sock, buf,year,month,now_client_sock);
	//fclose(fin);
	close(sock);
	return 0;
	
}
void read_file(int sock,char *buf)
{
	
	while(1)
	{
	int str_len=read(sock, buf, BUF_SIZE);
		if(str_len<0)
	return;

	buf[str_len]=0;
	
	printf("Read File: *%s*", buf);
	}


}


void read_routine(int sock, char *buf,int year, int month,int now_client_sock)
{
	
	
	while(1)
	{
	int str_len=read(sock, buf, BUF_SIZE);
	if(str_len==0)
	return;

	buf[str_len]=0;
	system("clear");
	make_cal(year,month,now_client_sock);
	printf("Message from server: %s", buf);
	}

}
void write_routine(int sock, char *buf,int now_client_sock)
{
	char mark[100];
	while(1)
	{

	fgets(buf, BUF_SIZE, stdin);
	if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
	{
		shutdown(sock, SHUT_WR);
		return;
	}
	if(!strcmp(buf,"make a memo\n") || !strcmp(buf, "Make a Memo\n"))
	{
		itoa(now_client_sock,mark);
		strcat(mark,".txt");
		FILE *fin = fopen(mark,"a");
		puts("Submit your memo ex)year month day data");
		fgets(buf,BUF_SIZE,stdin);
		fputs(buf,fin);
		fclose(fin);
	}
	write(sock, buf, strlen(buf));
	}

}

void make_cal(int year, int month, int sock){
	char mark[100];
	itoa(sock,mark);
	strcat(mark,".txt");
	if(access(mark,F_OK)==-1)
	{
		FILE* fin=fopen(mark,"w");
		fclose(fin);
	}
	FILE *fin=fopen(mark,"r");
	char year5[10], month5[10], day5[10];
	fscanf(fin,"%s",year5);
	fscanf(fin,"%s",month5);
	fscanf(fin,"%s",day5);
	int year4, month4, day4;
	year4=atoi(year5);
	month4=atoi(month5);
	day4=atoi(day5);
	fclose(fin);
	int right_day=-1;
	if(year4==year&&month4==month)
	{
		right_day=day4;
	}


	int i, yun = 0;
	int day, day1 = 0, day2 = 0, special = 0;
	int month_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	//윤년 구하기
	for (i = 1; i < year; i++)
		{
			if (i % 4 == 0 && !(i % 100 == 0))
		 	{
				special++;
				//printf("%d\n", i);
			}
			else if (i % 400 == 0)
			{
				special++;
				//printf("%d\n", i);
			}
															}
			//printf("윤년:%d번\n", special);
			/* 1)입력년도 전년도까지 일수 */
			day1 = 365 * (year - 1) + special;
			//printf("일수1:%d\n", day1);
			/* 2)입력년도 1월 1일부터 입력월 1일까지 일수 */
			for (i = 0; i < month - 1; i++)
			{
				day2 += month_day[i];
			}
			day2 += 1;
			//윤년일 때
			if ((year % 4 == 0 && !(year % 100 == 0)) || (year%400==0))
			{
				yun = 1;
				if (month>=3)
					day2 += 1; 
			}
			//윤년이 아닐 때
			else
			{
				yun = 0;
				day2 = day2;
			}
			//printf("일수2:%d\n", day2);
			/* 3) 1)과 2)를 더한 총일수 */
			day = day1 + day2;
			//printf("총 일수:%d\n", day);
			/* 제목 출력 */
			printf("\n\t  %d  %d\n", year, month);
			printf("\t============\n");
			printf("-----------------------------\n");
			printf(" SUN MON TUE WED THU FRI SAT\n");
			printf("-----------------------------\n");
			//시작 요일에 맞춰 정렬
			for (i = 0; i < day % 7; i++)
				{
					printf("    ");
				}
			int cnt = (day % 7);
			if (yun == 1 && month==2)//윤년 2월일 때
			{
				for (i = 1; i <= 29; i++)
				{
					cnt++;
					if(i==right_day)
					{
						printf("%c[1;33m", 27);
						printf("%4d",i);
						printf("%c[0m",27);

					}
					else
						printf("%4d", i);
					if (cnt % 7 == 0)
						printf("\n");
				}
			}
			else//그 외 일반 출력
			{
				for (i = 1; i <= month_day[month - 1]; i++)
				{
					cnt++;
					if(i==right_day)
					{
						printf("%c[1;33m", 27);
						printf("%4d",i);
						printf("%c[0m",27);

					}
					else
						printf("%4d", i);
					if (cnt % 7 == 0)
						printf("\n");
				}
			}
			printf("\n\n");
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void itoa(int n, char s[])

{

	int i, sign;

	if((sign = n) <0)

		n = -n;

	i=0;

	do {

		s[i++] = n%10 +'0';

	} while((n/=10)>0);

	if(sign<0)

		s[i++]='-';

	s[i]='\0';

	reverse(s);

}

void reverse(char s[])

{

	int c,i,j;

	for(i=0,j=strlen(s)-1;i<j;i++,j--)

	{

		c=s[i];

		s[i]=s[j];

		s[j]=c;

	}

}
