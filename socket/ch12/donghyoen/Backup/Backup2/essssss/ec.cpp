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
void read_routine(int sock, char *buf,int year, int month);
void write_routine(int sock, char *buf);
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
	write(sock, temp, strlen(temp));


	printf("\nID :");
	char ID[21];
	scanf("%s", ID);
	getchar();

	write(sock, ID, sizeof(ID));
	//ID?�송 




	printf("Year : ");
	char year2[21];
	scanf("%s", &year2);
	getchar();

	write(sock, year2, sizeof(year2));

	printf("Month : ");
	char month2[21];
	scanf("%s", &month2);
	getchar();

	write(sock, month2, sizeof(month2));

//	FILE *readfp;
//	readfp = fdopen(sock, "r");

//	strcat(ID, ".txt");
//	vector<CAL> my_cal;
//	FILE *fin = fopen(ID, "w");
/*
	while (1)
	{
		if (fgets(buf, sizeof(buf), readfp) == NULL)
			break;
		fputs(buf, stdout);
		fflush(stdout);
		fputs(buf, fin);
		fflush(fin);
	}
	//?�일 받기
*/
/*

	printf("before");
	fclose(fin);
	fin = fopen(ID, "r");
	printf("after");
	while (1)
	{
		if (fgets(temp, sizeof(temp), fin) == NULL)
			break;
		printf("%s", temp);
	}
*/


	//���� �޴� ��ƾ//
//	read_file(sock,buf);


//	printf("read_file_end\n");
	
	year = atoi(year2);
	month = atoi(month2);
	make_cal(year,month,i);

	pid = fork();
	if (pid == 0)
		write_routine(sock, buf);
	else
		read_routine(sock, buf,year,month);
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


void read_routine(int sock, char *buf,int year, int month)
{
	
	
	while(1)
	{
	int str_len=read(sock, buf, BUF_SIZE);
	if(str_len==0)
	return;

	buf[str_len]=0;
	system("clear");
	make_cal(year,month);
	printf("Message from server: %s", buf);
	}

}
void write_routine(int sock, char *buf)
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
		itoa(sock,mark);
		strcat(mark,".txt");
		FILE *fin = fopen(mark,"w");
		puts("Submit your memo ex)year month day data");
		fgets(buf,BUF_SIZE,stdin);
		fputs(buf,fin);
		fclose(fin);
	}
	write(sock, buf, strlen(buf));
	}

}

void make_cal(int year, int month,int sock){
	int i, yun = 0;
	int day, day1 = 0, day2 = 0, special = 0;
	int month_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	char ID[21];
	sprintf(ID,"%d.txt",sock);
	FILE* fin=(ID,"r");

	int year_sock;
	int month_sock;
	int day_sock;
	int right_day=-1;
	fscanf(fin,"%d",&year_sock);
	fscanf(fin,"%d",&month_sock);
	fscanf(fin,"%d",&day_sock);
	if(year_sock==year&&month_sock==month)
	{
		right_day=day_sock;
	}

	//���� ���ϱ�
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
			//printf("����:%d��\n", special);
			/* 1)�Է³⵵ ���⵵���� �ϼ� */
			day1 = 365 * (year - 1) + special;
			//printf("�ϼ�1:%d\n", day1);
			/* 2)�Է³⵵ 1�� 1�Ϻ��� �Է¿� 1�ϱ��� �ϼ� */
			for (i = 0; i < month - 1; i++)
			{
				day2 += month_day[i];
			}
			day2 += 1;
			//������ ��
			if ((year % 4 == 0 && !(year % 100 == 0)) || (year%400==0))
			{
				yun = 1;
				if (month>=3)
					day2 += 1; 
			}
			//������ �ƴ� ��
			else
			{
				yun = 0;
				day2 = day2;
			}
			//printf("�ϼ�2:%d\n", day2);
			/* 3) 1)�� 2)�� ���� ���ϼ� */
			day = day1 + day2;
			//printf("�� �ϼ�:%d\n", day);
			/* ���� ��� */
			printf("\n\t  %d  %d\n", year, month);
			printf("\t============\n");
			printf("-----------------------------\n");
			printf(" SUN MON TUE WED THU FRI SAT\n");
			printf("-----------------------------\n");
			//���� ���Ͽ� ���� ����
			for (i = 0; i < day % 7; i++)
				{
					printf("    ");
				}
			int cnt = (day % 7);
			if (yun == 1 && month==2)//���� 2���� ��
			{
				for (i = 1; i <= 29; i++)
				{
					cnt++;
					if(right_day==i)
					{
						printf("%c[1;33m", 27);
						printf("%4d", i);
						printf("%c[0m", 27);
					}
					else
					{
					
					printf("%4d", i);
					}


					if (cnt % 7 == 0)
						printf("\n");
				}
			}
			else//�� �� �Ϲ� ���
			{
				for (i = 1; i <= month_day[month - 1]; i++)
				{
					cnt++;
					if(right_day==i)
					{
						printf("%c[1;33m", 27);
						printf("%4d", i);
						printf("%c[0m", 27);
					}
					else
					{
					
					printf("%4d", i);
					}

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
