#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
int main(void)
{

	int fd=open("input.txt",O_RDONLY);
	char message[10];
	read(fd,message,1);

	

	message[10]=0;
	for(int i=0;i<10;i++)
		printf("%c",message[i]);
	return 0;
}
