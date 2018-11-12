//2015118653
//양동현 YangDongHyeon

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid=fork();//여기서 포크해서 자식프로세스
	//생성하고 pid값 넣음
	//부모 프로세스의 pid값은 자식프로세스의 프로세스 ID 이고 
	//자식 프로세스의 pid값은 0
	
	if(pid==0)     
	{
		puts("Hi I'am a child process");
	}
	else
	{
		printf("Child Process ID: %d \n", pid);
		sleep(30);  //부모는 printf를 실행하고 30초 블럭된다.
	}

	if(pid==0)
		puts("End child process");
	else
		puts("End parent process");
	return 0;
}
