#include <stdio.h>
#include <Windows.h>
#define LEN 10

int main()
{
	int a[LEN]={0,2,4,6,8,9,7,5,3,1};
	int min=45747456, temp=0, i, j;

	//이게 제일 낮은숫자 결과값 0 
	for( i = 0 ; i < LEN; i++)
	{
		if(a[i] < min )
		{
			min = a[i];
			printf("%d\n", min);
		}
	}

	for( j=0 ; j< LEN-1 ; j++)
	{
		//두번쨰로 낮은숫자 1
		for( i = 0, temp=0x7FFFFFFF ; i < LEN ; i++)
		{

			if(a[i] < temp && a[i] > min)
			{
				temp = a[i];					
			}
		}	
		min=temp;
		printf("%d\n", min);
	}

	system("pause");
}