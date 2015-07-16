#include <stdio.h>
//#include <Windows.h>

int main()
{
	int* padd1;
	int* padd2;
	int ivalue1 = 10;

	padd1 = &ivalue1;
	//padd2 = &padd1;

	printf("%d\n", padd1);
	printf("%d\n", *padd1);
	//printf("%d\n", **padd1);
	//system("pause");
	return 0;
}