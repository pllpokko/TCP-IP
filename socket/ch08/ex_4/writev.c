//2015118653
//양동현 yang dong hyeon

#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
	struct iovec vec[2];
	char buf1[]="ABCDEFG";
	char buf2[]="1234567";
	int str_len;

	vec[0].iov_base=buf1;//처음에 전송할 데이터가 저장된위치
	vec[0].iov_len=3;//처음에 전송할 데이터의 크기
	vec[1].iov_base=buf2;//두번째 전송할 데이터가 저장된 위치
	vec[1].iov_len=4;//두번 째 전송할 데이터의 크기
	
	str_len=writev(1, vec, 2);//fd 1에 vec에 내용된을 두번째 인자로 전달된 주소값이 가리키는
	//배열길이 정보 넣기
	puts("");
	printf("Write bytes: %d \n", str_len);
	return 0;
}
