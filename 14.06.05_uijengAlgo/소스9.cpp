#include<stdio.h>

int main()
{
	char *ch[] = {"Secmem", "Ssm", "Membership"};

	printf("%c ", *(*ch)++);
	printf("%c ", *(ch[1]));
	printf("%c ", *(ch[2])+1);
	printf("%c ", *(*ch)++);
	return 0;
}