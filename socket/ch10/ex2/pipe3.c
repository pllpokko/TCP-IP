//2015118653
//YangDongHyeon

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	char str1[]="Who are you?";
	char str2[]="Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fds1), pipe(fds2);//pipe를 만들고 파이프를 설정한다.

	pid=fork();//포크!
	
	if(pid==0)
	{
		//자식프로세스 
		write(fds1[1], str1, sizeof(str1));
		//자식프로세스에서 fds1[1]로 fds1[0]으로 전송한다.
		read(fds2[0], buf, BUF_SIZE);
		//fds2[0]으로 받은 메세지를 buf에 쓴다. 
		printf("Child proc output: %s \n",  buf);
		//받은 메세지를 printf로 띄운다. 
	}
	else
	{
		//부모프로세스 
		read(fds1[0], buf, BUF_SIZE);
		//fds1출력 파일디스크립터 부터 읽어 버퍼에 쓴다. 
		printf("Parent proc output: %s \n", buf);
		//받은 메세지를 printf로 띄운다 
		write(fds2[1], str2, sizeof(str2));
		//str2 내용을 fds2[1]을 이용해 fds2[0]으로 보낸다 
		sleep(3);
		//3초 블럭 
	}
	return 0;
}
