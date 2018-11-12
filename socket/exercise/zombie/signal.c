#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t pid;
void delzombie(int sig)
{
	int status;
	if(sig==SIGCHLD)
	{
		waitpid(pid,&status,WNOHANG);
		if(WIFEXITED(status))
			printf("child return : %d \n",WEXITSTATUS(status));
	}
		
}

int main(void)
{

	signal(SIGCHLD,delzombie);

	pid=fork();
	if(pid==0)
	{
		sleep(5);
		return 5;
	}
	else
	{
		sleep(100);
		printf("child end\n");

	}

	return 0;
}
