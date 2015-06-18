#include <stdio.h>

void swap(int *a, int **b)
{
	int * cc;
	int ccc = 3;
	unsigned temp;
	cc = &ccc;

	//temp = *a;
	//*a = *b;
	//*b = temp;
	b = &cc;

}

int main()
{
	int aa = 10, bb = 20;
	int *a = 0;
	int *b = 0;
	a = &aa;
	//b = &bb;

	//printf("%d %d\n", *a, *b);
	swap(a, &b);
	printf("%d %d\n", *a, *b);

	return 0;
}