//2015118653
//양동현 Yang Dong Hyeon

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid=fork();//포크
	
	if(pid==0)//자식프로세스 
	{
		sleep(15);//15초 슬립
		return 24;   	//24리턴
	}
	else//부모프로세스
	{
		while(!waitpid(-1, &status, WNOHANG))
		{//-1 하면 wait 처럼 임의의 자식 프로세스가 종료되길 기다린다.
			//WNOHANG은 부모 프로세스를 wait처럼 블로킹 시키지 않는다. 
			//아직 종료된 자식 프로세스가 없다면 0을 리턴한다.
			//아직 종료된 자식 프로세스가 없다면
			//아래 문장을 실행한다.
			sleep(3);
			puts("sleep 3sec.");
		}

		if(WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}
	return 0;
}

