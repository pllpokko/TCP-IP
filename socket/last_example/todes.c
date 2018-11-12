//2015118653
//YangDongHyeon

#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	FILE *fp;
	int fd=open("data.dat", O_WRONLY|O_CREAT|O_TRUNC);
	//open으로 "data.dat"파일을 OW_WRONLY(읽고쓰기) O_CREAT(없으면 파일 만들기)등의 옵션으로
	//파일디스크립터를 반환한다. 
	if(fd==-1)
	{
		fputs("file open error", stdout);
		return -1;
	}
	
	printf("First file descriptor: %d \n", fd); 
	//처음 open으로 만든 파일디스크립터를 보여준다.

	fp=fdopen(fd, "w");
	//표준입출력인 파일포인터를 사용해보자.
	//fp는 파일포인터인데 fd는 파일디스크립터이다. 이를 파일포인터로 변경한다.
	//"w"옵션을 주어서 여기다 '쓴다'라고 옵션을 준다. 
	fputs("TCP/IP SOCKET PROGRAMMING \n", fp);
	//fp에 문자열을 기록한다. 
  	printf("Second file descriptor: %d \n", fileno(fp));
	//fp가 담긴 파일디스크립터를 출력한다. 
	fclose(fp);
	return 0;
}
