//2015118653
//양동현 YangDongHyeon
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
void keycontrol(int sig)
{
	if(sig==SIGINT)
		printf("Do you want to close this program? (y|n) ");	
	char y_n;
	scanf("%c",&y_n);
	getchar();//엔터 키를 잡아 먹음 (버퍼 비움)
	if(y_n=='y')
	{
		exit(0); //y면 종료 
	}
}

int main(int argc, char *argv[])
{

	//signal(SIGINT, keycontrol);
	struct sigaction act;
	act.sa_handler=keycontrol;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGINT,&act,0);
	while(1)
	{
		printf("This program is running\n"); 
		sleep(1);
	}
	return 0;
}
