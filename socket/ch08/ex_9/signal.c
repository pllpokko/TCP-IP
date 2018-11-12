//2015118653
//양동현 YangDongHyeon
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	//들어온 신호가 SIGALRM(알람이 울렸으면) Time out!을 출력 
	alarm(2);	//다시 알람을 설정
}
void keycontrol(int sig)
{
	if(sig==SIGINT)
		puts("CTRL+C pressed");
}

int main(int argc, char *argv[])
{
	int i;
	signal(SIGALRM, timeout);
	//시그널 함수를 등록한다.
	//알람이 울리면 실행할 함수를 뒤 매개변수에 등록한다.

	signal(SIGINT, keycontrol);
	//SIGINT는 ctrl+C가 입력된 상황이다.
	//입력되면 keycontrol함수실행 
	alarm(2);

	for(i=0; i<3; i++)
	{
		puts("wait...");//100초 슬립되는 상황이지만 
		sleep(100);//30번째 줄의 alarm 때매 2초마다 슬립된
		//이 프로세스를 깨운다.
	}
	return 0;
}
