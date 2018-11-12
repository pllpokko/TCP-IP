//2015118653
//양동현 YangDongHyeon
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	alarm(2);	
}

int main(int argc, char *argv[])
{
	int i;
	struct sigaction act;
	//sigaction을 사용하면
	//signal 함수보다 유용하다.
	//signal 함수는 유닉스 계열의 운영체제 별로 동작방식에
	//차이가 있기 때문

	act.sa_handler=timeout; //시그날이 오면 함수실행 할 것 
	sigemptyset(&act.sa_mask);//모든비트 0 초기화
	act.sa_flags=0;
	sigaction(SIGALRM, &act, 0);//SIGALRM이 울리면 &act에 등록된대로 해라고
	//시그널 함수를 등록한다.

	alarm(2);//2초 알람 셋

	for(i=0; i<3; i++)
	{
		puts("wait...");
		sleep(100);
	}
	return 0;
}
