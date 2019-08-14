#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int main(int argc, const char *argv[])
{
	char a[20];
	char b[200];
	FILE* fr = fopen("./dict.txt", "r+");
	if (NULL == fr)
	{
		perror("open fail:");
		exit(-1);
	}
	
	

/*
	while (feof(fr) == 0)
	{
		fread(&a, sizeof(a), 1, fr) == 1;
		if (feof(fr) != 0)
		{
			break;
		}
		fread(&a, sizeof(a), 1, fr);
		printf("a = %s,b = %s\n",a,b);
	}
*/
}
