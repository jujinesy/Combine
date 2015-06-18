#include<stdio.h>

int main()
{
	int sum = 0, cnt = 0;;
	while (cnt <= 99)
	{
		cnt++;
		sum=sum+cnt;
	}
	printf("%d", sum);
	return 0;
}