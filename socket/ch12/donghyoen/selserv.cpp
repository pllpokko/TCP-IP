//2015118653
//�絿��

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[])
{
	FILE *fin;
	int serv_sock, clnt_sock; //���� ��ũ���� ����
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout; //Ÿ�Ӿƿ� ����ü ����
	fd_set reads, cpy_reads; //fd_set�� ���� �ΰ��� �����. �ϳ��� copy���̴�.
	int fd;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	//fd_max�� �˻��� ���ϵ�ũ���� ������ �ִ´�.
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	FD_ZERO(&reads);//reads�� ���� ���ϵ�ũ���͸� ���� 0���� �����. �� �ʱ�ȭ�Ѵ�.
	FD_SET(serv_sock, &reads);//serv_sock���� ���ϵ�ũ���� ��ȣ�� �ִµ�
	//reads���� serv_sock�� �´� ���ϵ�ũ���� ��ȣ(��ġ)�� 1�� ���Ѵ�.
	//�� ���� ���Ͽ� ��ȭ�� �ִ��� ������ �ΰ� ���Ѻ��ٴ� ���̴�.
	fd_max=serv_sock;
	//�˻��� ���ϵ�ũ���� ������ serv_sock�� �ִ´�.

	while(1)
	{
		cpy_reads=reads; //cpy_reads�� reads�� �ְ� ������ select�� �� copy���� ����.
		//������ select �Լ� ȣ���� ������ ��ȭ�� ���� ���� ��ũ������ ��ġ�� ������ ������ ��ġ�� ��Ʈ����
		//0���� �ʱ�ȭ �ȴ�. ���� ������ ������ ���ؼ� ���纻�� ����Ѵ�.
		timeout.tv_sec=5;
		timeout.tv_usec=5000;
		//select �Լ�ȣ�� ���� Ÿ�Ӿƿ����� �����Ѵ�.
		//select �Լ� ȣ�� �Ŀ��� ����ü timeval�� ��� tv_sec�� tv_usec�� ����� ���� Ÿ�Ӿƿ��� �߻��ϱ� ����
		//���Ҵ� �ð����� �ٲ�� �����̴�. ���� select �Լ��� ȣ���ϱ� ���� �Ź� timeval ����ü ������ �ʱ�ȭ��
		//�ݺ��ؾ��Ѵ�. 
		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
			break;
		//select�Լ��� ȣ���Ѵ�.
		//ù ��° �Ű������� �ִ� �˻��� ���ϵ�ũ���� �����̴�. 0~fd_max ���� �� (fd_max+1)�� �˻��Ѵ�.
		//fd_max���� serv_sock�� �����Ƿ� 0~���� �������ϵ�ũ���ͱ��� �˻��Ѵ�.

		//�� ��° �Ű������� fd_set* readset�̴�.
		//fd_set�� ������ '���ŵ� �������� ���翩��'�� ������ �ִ� ���� ��ũ���� ������ ��� ����ؼ� �� ������
		//�ּ� ���� �����Ѵ�.

		//�� ��° �Ű������� fd_set* writeset�̴�.
		//fd_set�� ������ '���ŷ ���� ������ ������ ���ɿ���'�� ���� �ִ� ���� ��ũ���� ������ ��� ����ؼ�
		//�� ������ �ּ� ���� �����Ѵ�.

		//�� ��° �Ű������� fd_set *exceptset�̴�.
		//fd_set�� ������ '���ܻ�Ȳ�� �߻�����'�� ������ �ִ� ���� ��ũ���� ������ ��� ����ؼ�
		//�� ������ �ּ� ���� �����Ѵ�.

		//�ټ� ��° �Ű������� select �Լ� ȣ�� ���Ŀ� ������ ���ŷ ���¿� ������ �ʵ��� Ÿ�Ӿƿ��� �����ϱ� ���� ����
		//�� ���Ѵ�.

		//��ȯ���� ���� �߻��ÿ� -1, Ÿ�� �ƿ��� ���� ��ȯ �ÿ��� 0, ���� ������� ��ϵ� ���� ��ũ���Ϳ� �ش��ϴ�
		//���� ��ũ���Ϳ� ��ȭ�� �߻��ϸ� ��ȭ�� �߻��� ���� ��ũ������ ���� ��ȯ�Ѵ�.


		
		if(fd_num==0)
			continue;
		//Ÿ�� �ƿ��� �߻��� ��� fd_num�� 0�� ���µ� �׷��� continue ����

		for(i=0; i<fd_max+1; i++)//fd_num�� 1�̻� ��ȯ�Ǿ��� �� �����ϴ� �ݺ����̴�.
		{ 
			//0~���ϵ�ũ���ͱ��� üũ�Ѵ�.
			
			if(FD_ISSET(i, &cpy_reads))//cpy_reads���� i��° ���ϵ�ũ���Ͱ� �µ��ִٸ� 1�� ��ȯ�ȴ�.
			{//���ŵ� �����Ͱ� �ִ� ���� ��ũ���͸� ã�´�.
				if(i==serv_sock)     
				{
					//���º�ȭ�� Ȯ���� �Ǹ� ���� ���� ���� ���Ͽ��� ��ȭ�� �־����� Ȯ���Ѵ�.
					//���� ������ ���º�ȭ�� �´ٸ�
					adr_sz=sizeof(clnt_adr);
					clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					//Ŭ���̾�Ʈ�� accept ������ ����.
					
					FD_SET(clnt_sock, &reads);
					if (fd_max<clnt_sock)
						fd_max = clnt_sock;
					//clnt_sock�� reads�� �׿� �´� ���ϵ�ũ���Ϳ� 1�� ���Ѵ�.
					//������ reads�� clnt_sock��ġ�� 1�� �������Ƿ� �� Ŭ���̾�Ʈ��
					//��û�� ���� ���  �Ʒ� else���� �����Ѵ�.
					
					printf("connected client: %d \n", clnt_sock);
				}
				else    // read message!
				{
					//���º�ȭ�� �߻��� ������ ���� ������ �ƴ� ��쿡 ����ȴ�.
					//�� ������ �����Ͱ� �ִ� ��쿡 ����ȴ�.
					str_len=read(i, buf, BUF_SIZE);
					buf[str_len]=0;
					if(str_len==0)   
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i);
						//st_len�� 0�̸� ������ ��������
						//FD_CLR�� i(clnt sock ���ϵ�ũ����)�� 0���� �����.
						//�׸��� clnt�� �����Ѵ�.
					}
					else
					{


						//modified���
						if(!strcmp("modified",buf))
						{
							char ID[21];
							str_len=read(i,ID,sizeof(ID));
							ID[str_len]=0;
							strcat(ID,".txt");
							fin=fopen(ID,"r");
							if(fin==NULL){
								fd = open(ID, O_CREAT|O_RDWR|O_APPEND);
								fin = fdopen(fd,"r");
							}
							while(!feof(fin))
							{
								char str[4][500];
								fscanf(fin,"%s",str[0]);
								fscanf(fin,"%s",str[1]);
								fscanf(fin,"%s",str[2]);
								fscanf(fin,"%s",str[3]);
								//fscanf(fin,"%s",str[4]);
								strcpy(buf,"*");
								strcat(buf,str[0]);
								strcat(buf,str[1]);
								strcat(buf,str[2]);
								strcat(buf,"\n");
								//char *result;
								//if((result=fgets(buf,BUF_SIZE,fin))!=NULL);
								//strcat(result,"\n");
								str_len=write(i,buf,sizeof(buf));
							}
							fclose(fin);
							char year[21];
							str_len=read(i,year,sizeof(buf));
							year[str_len]=0;

							char month[21];
							str_len=read(i,month,sizeof(buf));
							month[str_len]=0;
							
							//for debug
							printf("ID : %s\n",ID);
							printf("year : %s\n",year);
							printf("month : %s\n",month);
							
							//���� ����

							




						}

						else{
						for(int i=4;i<fd_max+1;i++)
						{
							write(i,buf,str_len);
						}
						}
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
