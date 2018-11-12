//2015118653
//양동현 Yang Dong Hyeon

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid=fork(); //포크한다.
	
	if(pid==0)
	{
		return 3;   	//자식프로세스는 리턴3을 
		//운영체제에 올리고 종료된다.
	}
	else
	{
		printf("Child PID: %d \n", pid);//부모 프로세스의 pid에는
		//자식 프로세스의 프로세스ID값이 저장된다. 이를 출력한다.
		pid=fork(); //또 포크한다.
		if(pid==0)
		{
			exit(7); //여기서 자식 프로세스는 7을 운영체제에 올리고 
			//종료된다.
		}
		else
		{
			printf("Child PID: %d \n", pid);//부모프로세스는
			//방금 만든 자식프로세스의 프로세스ID를 출력한다.
			wait(&status);//wait를 호출한다.
			//이로 인해 종료된 프로세스 관련정보는 status에 담기게 된다.
			//자식프로세스의 리턴값은 wait에서 받아서 status에 저장한다.
			//이로써 자식 프로세스는 종료된다. 
			
			if(WIFEXITED(status))//
				printf("Child send one: %d \n", WEXITSTATUS(status));
			//WEXITSTATUS(status)로 status 값을 참고
			//이때 status에는 자식프로세스에서 return, exit 값이다.
			wait(&status);
			if(WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			//위와 같다.

			sleep(30);     // 30초 sleep 시킨다.
		}
	}
	return 0;
}
