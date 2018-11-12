#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>


#define BUF_SIZE 1024
void error_handling(char *message);

using namespace std;
typedef struct
{
	int year;
	int month;
	int day;
}CAL;

int main()
{
	FILE* fin;
	FILE* fout;
	fin=fopen("my_cal.txt","r");
	fout=fopen("my_cal_out.txt","wa");
	int i, yun = 0;
	int year, month;
	int day, day1 = 0, day2 = 0, special = 0;
	int month_day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	//printf("년, 월을 입력하세요:");
	//scanf("%d %d", &year, &month);


	//귀찮아서 기본값 year month
	year=2018;
	month=6;
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

	vector<CAL> my_cal;
	while (!feof(fin))
	{
		int temp_year, temp_month, temp_day;
		fscanf(fin, "%d %d %d", &temp_year, &temp_month, &temp_day);
		CAL temp_cal = { temp_year,temp_month,temp_day };
		my_cal.push_back(temp_cal);
	}


	//출력
while(1)
{

	system("clear");

	//나의 일정중 해당하는 year,month로 찾기

	vector<CAL> this_Year_Month;

	for (int i = 0; i < my_cal.size(); i++)
	{
		if (my_cal[i].year == year)
		{
			if (my_cal[i].month == month)
			{
				CAL temp = { my_cal[i].year,my_cal[i].month,my_cal[i].day };
				this_Year_Month.push_back(temp);
			}
		}
	}

	


	/* 제목 출력 */
	printf("\n\t  %d년  %d월\n", year, month);
	printf("\t==============\n");
	printf("-----------------------------\n");
	printf(" SUN MON TUE WED THU FRI SAT\n");
	
	

	printf("-----------------------------\n");


	//시작 요일에 맞춰 정렬
	for (int i = 0; i < day % 7; i++)
	{
		printf("    ");
	}
	
	int cnt2 = 0;

	int cnt = (day % 7);

	if (yun == 1 && month==2)//윤년 2월일 때
	{
		for (int i = 1; i <= 29; i++)
		{
			cnt++;
			if (this_Year_Month[cnt2].day == i)
			{
				printf("%c[1;33m", 27);
				printf("%4d", i);
				printf("%c[0m", 27);
				if (cnt2 < this_Year_Month.size())
					cnt2++;
			}
			else
			{
				printf("%4d", i);
			}
			if (cnt % 7 == 0)
				printf("\n");
		}
	}
	else//그 외 일반 출력
	{
		for (int i = 1; i <= month_day[month - 1]; i++)
		{
			cnt++;
			if (this_Year_Month[cnt2].day == i)
			{
				printf("%c[1;33m", 27);
				printf("%4d", i);
				printf("%c[0m", 27);
				if (cnt2 < this_Year_Month.size())
					cnt2++;
			}
			else
				printf("%4d", i);
			if (cnt % 7 == 0)
				printf("\n");
		}
	}
	printf("\n\n");

	getchar();

}
fclose(fin);
fclose(fout);

}
