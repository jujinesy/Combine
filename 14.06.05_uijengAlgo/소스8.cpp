#include<stdio.h>

int main()
{
	int x =1 , y= 2, z[10] = {3,};
	int *ip;

	ip = &x;
	y = *ip;
	*ip = 0;
	ip = &z[8];

	printf("%d %d %d", x, y, *ip);
	return 0;
}