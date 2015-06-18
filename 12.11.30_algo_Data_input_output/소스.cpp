#include<stdio.h>

FILE *tempFile;
char str['ab'];

int main()
{
	freopen_s(&tempFile, "input.txt", "r", stdin);
	freopen_s(&tempFile, "output.txt", "w", stdout);

	scanf("%s", str);
	printf("%s", str);
	return(0);
}