#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	char *str[3]={"hello?","nice to meet you","welcome"};



	int message_int[3];
	for(int i=0;i<sizeof(str);i++)
	{
		message_int[i]=strlen(str[i]);
	}

	
	return 0;
}
