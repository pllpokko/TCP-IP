#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(void)
{
	int fd1[2],fd2[2];

	char str1[]="This from Parents";
	char str2[]="This from Child";

	char buf[BUF_SIZE];

	pid_t pid;
	pipe(fd1);
	pipe(fd2);

	pid=fork();
	if(pid==0)
	{

		read(fd1[0],buf,BUF_SIZE);
		puts(buf);
		write(fd2[1],str2,sizeof(str2));
	}
	else
	{

		write(fd1[1],str1,sizeof(str1));
		
		read(fd2[0],buf,BUF_SIZE);
		puts(buf);
	}
	return 0;
}
