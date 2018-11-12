//2015118653
//양동현 YangDongHyeon

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
	int status;
	pid_t id=waitpid(-1, &status, WNOHANG);
	//waitpid 함수의 정의대로
	//-1 <-- 임의의 자식프로세스 종료 기다림
	//WNOHANG으로 부모프로세스를 블로킹 시키지 않는다.
	

	if(WIFEXITED(status))
	{
		//WIFEEXITED로 status값이 있으면 
		//즉 종료된 자식프로세스가 있으면 아래 문장 실행 
		printf("Removed proc id: %d \n", id);
		//제거된 프로세스 아이디 출력 
		printf("Child send: %d \n", WEXITSTATUS(status));
		//자식프로세스가 종료하면서 return,exit 한 값 출력 
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGCHLD, &act, 0);
	//SIGCHLD 는 종료된 자식 프로세스가 있다면
	//시그널을 보내는 것이다.
	//sigaction 함수는 SIGCHLD 즉 자식프로세스가 종료되면
	//act를 실행시키는데 
	//위에 act에 등록한대로 read_childproc 함수를 실행한다. 

	pid=fork(); //포크 
	if(pid==0)
	{//자식프로세스 실행영역
		puts("Hi! I'm child process");
		sleep(10); //10초간 슬립하고 
		return 12; //12를 리턴한다 
	}
	else
	{
		printf("Child proc id: %d \n", pid);
		//부모프로세스 실해영역 
		//자식프로세스 pid를 찍어본다. 
		pid=fork(); //포크 
		if(pid==0)
		{
			//자식프로세스 실행영역 
			puts("Hi! I'm child process");
			sleep(10); //10초 슬립 
			exit(24); //24를 리턴 
		}
		else
		{
			int i; //부모프로세스 
			printf("Child proc id: %d \n", pid);
			//위의 자식프로세스 id를 찍는다.
			for(i=0; i<5; i++)
			{
				puts("wait..."); 
				sleep(5);//5초씩 슬립한다. 
			}
		}
	}
	return 0;
}

