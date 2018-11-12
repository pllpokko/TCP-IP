//2015118653
//YangDongHyeon

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fd[2];//파일디스크립터를 두개 만든다.
	//fd[0]은 출구이고
	//fd[1]은 입구이다.

	char str[]="Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fd);
	pid=fork();
	if(pid==0)
	{
		//자식 프로세스에서
		//fd[1] 즉 PIPE의 입력을 쓰고 str을 보낸다. 
		write(fd[1], str, sizeof(str));
	}
	else
	{
		//부모프로세스에서 fd[0] 즉 출구이고
		//fd[0]으로 받은 것을 buf에 넣는다.
		//그것을 puts로 출력한다.
		read(fd[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}
