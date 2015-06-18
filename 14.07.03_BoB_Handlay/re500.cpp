#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char buf[0x400];
	if (argc != 2)
	{
		puts("error");
		exit(0);
	}

	strcpy(buf, argv[1]);
	printf(buf);
	putchar('\n');
	return 0;
}