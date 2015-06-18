#include <stdio.h>

int main()
{
	int i = 0, j = 0;
	char word = 'A';

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < i; j++)
		{
			//
			printf("%c", word++);
		}
		printf("%c", word);
		while (word != 'A')
		{
			printf("%c", --word);
		}
		printf("\n");
	}
	getchar();
	return 0;
}