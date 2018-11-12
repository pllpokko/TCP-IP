//2015118653
//양동현 Yang Dong Hyeon
#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	struct iovec vec[2];
	char buf1[BUF_SIZE]={0,};
	char buf2[BUF_SIZE]={0,};
	int str_len;

	vec[0].iov_base=buf1;//첫번째 저장할 배열
	vec[0].iov_len=5;//그 길이
	vec[1].iov_base=buf2;//두번째 저장할 배열
	vec[1].iov_len=BUF_SIZE;//그 길이 

	str_len=readv(0, vec, 2);
	//0의 파일디스크립터에서 vec에 저장하는데 마지막 매개변수는
	//두번째 저장할 배열의 길이 정보이다.

	printf("Read bytes: %d \n", str_len);
	printf("First message: %s \n", buf1);
	printf("Second message: %s \n", buf2);
	return 0;
}

