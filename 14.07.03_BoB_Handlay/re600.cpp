#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int v[0x1a];

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		exit(0);
	}
	v[0] = 0x64;
	memset((int*)v, 0, 0x1a);
	strcpy((char*)(v + 4), argv[1]);
	puts((char*)(v + 4));
	return 0;
}