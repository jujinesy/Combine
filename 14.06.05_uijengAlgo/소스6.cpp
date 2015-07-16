#include <stdio.h>

int main()
{
	int a = 3;
	int b = 2;
	int c = 1;
	switch( c+= 3 & b | a && c)
	{
	case 1:
		printf("1");
		break;
	case 2:
		printf("2");
		break;
	case 3:
		printf("3");
		break;
	}

	printf(" %d %d %d",a ,b, c);

	return 0;
}
