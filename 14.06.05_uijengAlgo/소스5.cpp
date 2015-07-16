#include <stdio.h>

int main()
{
	int a = -1;
	long b = -1;
	unsigned c = 1;
	unsigned long d = 1;

	int* aa;
	long* bb;
	unsigned* cc;
	unsigned long* dd;

	aa = &a;
	bb = &b;
	cc = &c;
	dd = &d;

	printf("%d %d %d %d", a > c, b > c, a > d, b < d);

	return 0;
}