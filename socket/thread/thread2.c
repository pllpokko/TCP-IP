#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
void* thread_main(void *arg);
void* thread_main2(void *arg);
void* thread_main3(void *arg);
void* thread_main4(void *arg);

int main(int argc, char *argv[]) 
{
	pthread_t t_id[4];
	int thread_param=5;
	void * thr_ret;




		pthread_create(&t_id[0], NULL, thread_main, (void*)&thread_param);
		pthread_create(&t_id[1], NULL, thread_main2, (void*)&thread_param);
		pthread_create(&t_id[2], NULL, thread_main3, (void*)&thread_param);
		pthread_create(&t_id[3], NULL, thread_main4, (void*)&thread_param);

	pthread_join(t_id[0],&thr_ret);
	pthread_join(t_id[1],&thr_ret);
	pthread_join(t_id[2],&thr_ret);
	pthread_join(t_id[3],&thr_ret);

	printf("Thread return message: %s \n", (char*)thr_ret);
	free(thr_ret);
	return 0;
}
void* thread_main(void *arg) 
{
	int i;
	int cnt=*((int*)arg);
	char * msg=(char *)malloc(sizeof(char)*50);
	strcpy(msg, "Hello, I'am thread~ \n");

	for(i=0; i<cnt; i++)
	{
		sleep(1);  puts("running thread   1   ");	 
	}
	return (void*)msg;
}
void* thread_main2(void *arg) 
{
	int i;
	int cnt=*((int*)arg);
	char * msg=(char *)malloc(sizeof(char)*50);
	strcpy(msg, "Hello, I'am thread~ \n");

	for(i=0; i<cnt; i++)
	{
		sleep(3);  puts("running thread   2   ");	 
	}
	return (void*)msg;
}
void* thread_main3(void *arg) 
{
	int i;
	int cnt=*((int*)arg);
	char * msg=(char *)malloc(sizeof(char)*50);
	strcpy(msg, "Hello, I'am thread~ \n");

	for(i=0; i<cnt; i++)
	{
		usleep(200);  puts("running thread   3   ");	 
	}
	return (void*)msg;
}
void* thread_main4(void *arg) 
{
	int i;
	int cnt=*((int*)arg);
	char * msg=(char *)malloc(sizeof(char)*50);
	strcpy(msg, "Hello, I'am thread~ \n");

	for(i=0; i<cnt; i++)
	{
		  puts("running thread    4   ");	 
	}
	return (void*)msg;
}
