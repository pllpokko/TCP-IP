#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
	int status;
	pid_t pid=fork();
	pid_t pid2;
	if(pid==0)
	{
		sleep(6);
		return 3;
	}
	else
	{
		
		pid2=fork();
		if(pid2==0)
		{
			sleep(5);
			return 7;
		}
		else
		{
			while(!waitpid(pid,&status,WNOHANG))
			{
			}

			if(WIFEXITED(status))
					printf("pid : %d was returned : %d \n",pid,WEXITSTATUS(status));
			if(waitpid(pid2,&status,WNOHANG))
			{
				if(WIFEXITED(status))
					printf("pid : %d was returned : %d \n",pid2,WEXITSTATUS(status));
			}
		}
	}
	return 0;
}

