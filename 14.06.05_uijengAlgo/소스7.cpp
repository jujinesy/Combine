#include<stdio.h>

#define dprint(expr) printf(#expr" %d\n",expr);

int main()
{
	int x=20, y=10;

	dprint(x);

	return 0;
}